#include "includes.h"
#include "seedbox.h"
#include "utils.h"
#include "socket.h"

const char *HTTP_BODY = "HTTP/1.1 %d %s\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s\n";
const char *HTTP_RESPONSE_OK_BODY  = "Your file is now being downloaded...";
const char *HTTP_RESPONSE_ERR_BODY = "You don't provided a filename= query parameter";

Seedbox::Seedbox(char *port, map<CLI_ARGS,char*> userConf){
  sprintf(
    this->HTTP_RESPONSE_OK,
    HTTP_BODY,
    200,
    "OK",
    strlen(HTTP_RESPONSE_OK_BODY),
    HTTP_RESPONSE_OK_BODY
  );

  sprintf(
    this->HTTP_RESPONSE_ERR,
    HTTP_BODY,
    400,
    "Bad request",
    strlen(HTTP_RESPONSE_ERR_BODY),
    HTTP_RESPONSE_ERR_BODY
  );

  this->conf = userConf;

  this->listend(port);
};

void Seedbox::listend(char *port){
  Socket sock(port, this);
};

bool Seedbox::initDownload(char *data){
  char filename[1024];
  bool hasFilename = this->getFilename(data, filename);

  if(hasFilename == false){
    d.log("Could find filename attribute in request");
    return false;
  }

  fileData *file = (fileData*)malloc(sizeof(fileData));

  strcpy(file->filename, filename);

  strcpy(file->ftpURL, "ftp://");
  strcat(file->ftpURL, this->conf[ftpUser]);
  strcat(file->ftpURL, ":");
  strcat(file->ftpURL, this->conf[ftpPassword]);
  strcat(file->ftpURL, "@");
  strcat(file->ftpURL, this->conf[ftpHost]);
  strcat(file->ftpURL, "/");

  strcpy(file->ftpFilePath, this->conf[ftpBaseDir]);
  strcat(file->ftpFilePath, filename);

  strcpy(file->fullPath, this->conf[CLI_ARGS::savePath]);
  strcat(file->fullPath, filename);

  file->savePath = this->conf[CLI_ARGS::savePath];

  bool ret;

#ifdef _WIN32
  ret = this->createWindowsThread(file) == NULL ? false : true;
  Sleep(1 * 1000);
#else
  ret = this->createUNIXThread(file) == 0 ? true : false;
  sleep(1);
#endif

  if(ret){
    d.log("Download of", filename, "is on its way...");
  } else{
    d.log("Error while downloading", filename);
  }

  return ret;
};

#ifdef _WIN32
DWORD WINAPI download(LPVOID arg) {
#else
void *download(void *arg) {
#endif
  fileData *file = (fileData*)arg;

  Debug d;
  d.log("Saving to", file->savePath);

  char command[2048];
#ifdef _WIN32
  strcpy(command, "cd ");
  strcat(command, file->savePath);
  strcat(command, " && ");
  strcat(command, "winscp.com /command \"open ");
  strcat(command, file->ftpURL);
  strcat(command, "\" \"get \"\"");
  strcat(command, file->ftpFilePath);
  strcat(command, "\"\"\" \"exit\" > NUL");
#else
  strcpy(command, "wget -nH --cut-dirs=1 -r -q -P ");
  strcat(command, file->savePath);
  strcat(command, " ");
  strcat(command, file->ftpURL);
  strcat(command, file->ftpFilePath);
#endif

  cout << "COMMAND: " << command << endl;
  system(command);

  // Here, we don't know yet if the filename is a whole folder or not
  // So we check if a file has been downloaded
  // If not, we try again with /*
  d.log("Checking full path:", file->fullPath);
  if(!fileExists(file->fullPath)){
    // TODO: this check is useless on windows as long as we use winscp
#ifdef _WIN32
    d.log("File", file->filename, "doesn't exist, wrong name ?");
#else
    if(file->isFolder){
      d.log("File", file->filename, "is not a file nor a folder, wrong name ?");
    } else{
      d.log("File", file->filename, "doesn't exist, trying as a folder..");
      strcat(file->ftpURL, "/*");
      file->isFolder = true;
      download(file);
    }
#endif
  } else{
    d.log("Download of", file->filename, "completed !");
  }

  free(file);
#ifdef _WIN32
  d.log("Exiting thread...");
  ExitThread(NULL);
#else
  pthread_exit(NULL);
#endif
};

bool Seedbox::getFilename(char *data, char *buffer){
  vector<char*> url = Utils::split(data, ' ', 0);
  if (url.size() <= 1) {
    return false;
  }
  vector<char*> qs = Utils::split(url[1], '?', 0);
  if (qs.size() <= 1) {
    return false;
  }
  vector<char*> params = Utils::split(qs[1], '&', 0);

  vector<char*>::iterator it;
  for(it = params.begin(); it != params.end(); it++){
    vector<char*> tmp = Utils::split(*it, '=', 0);
    if(strcmp(tmp[0], "filename") == 0){
      strcpy(buffer, tmp[1]);
      return true;
    }
  }

  return false;
};

#ifdef _WIN32
bool fileExists(char *path)
{
  WIN32_FIND_DATA FindFileData;
  HANDLE handle = FindFirstFile(path, &FindFileData);
  int found = handle != INVALID_HANDLE_VALUE;
  if (found)
  {
    //FindClose(&handle); this will crash
    FindClose(handle);
  }
  return found ? true : false;
}
#else
bool fileExists(char *path){
  ifstream file(path);
  bool exists = file.good();
  file.close();
  return exists;
};
#endif
bool Seedbox::onData(char *data){
  d.log("On data(), init download");
  return this->initDownload(data);
};

#ifdef _WIN32
HANDLE WINAPI Seedbox::createWindowsThread(fileData *file) {
	DWORD th;
	return CreateThread(NULL, 0, download, file, 0, &th);
};
#else
int Seedbox::createUNIXThread(fileData *file) {
	pthread_t th;
	return pthread_create(&th, NULL, download, file);
};
#endif
