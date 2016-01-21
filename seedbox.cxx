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

  strcpy(this->savePath, "/tmp/seedbox-daemon/");
  this->conf = userConf;

  this->listend(port);
};

void Seedbox::listend(char *port){
  Socket sock(port, this);
};

int Seedbox::initDownload(char *data){
  char filename[1024];
  bool hasFilename = this->getFilename(data, filename);

  if(hasFilename == 0){
    cout << "Could find filename attribute in request" << endl;
    return 1;
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
  strcat(file->ftpURL, this->conf[ftpBaseDir]);
  strcat(file->ftpURL, filename);

  strcpy(file->fullPath, this->savePath);
  strcat(file->fullPath, filename);

  d.log("FTP User", this->conf[ftpUser]);

  file->savePath = this->conf[CLI_ARGS::savePath];

  pthread_t th;
  int thread = pthread_create(&th, NULL, download, file);

  sleep(1);

  if(thread == 0){
    cout << "Download of " << filename << " is on its way..." << endl;
  } else{
    cout << "Error while downloading " << filename << endl;
  }

  return thread;
};

void *download(void *arg){
  fileData *file = (fileData*)arg;

  Debug d;

  char command[2048];
  strcpy(command, "wget -nH --cut-dirs=1 -r -q -P ");
  strcat(command, file->savePath);
  strcat(command, " ");
  strcat(command, file->ftpURL);
  cout << "COMMAND: " << command << endl;
  system(command);

  // Here, we don't know yet if the filename is a whole folder or not
  // So we check if a file has been downloaded
  // If not, we try again with /*
  cout << "Checking full path: " << file->fullPath << endl;
  if(!fileExists(file->fullPath)){
    if(file->isFolder){
      cout << "File is not a file nor a folder, wrong name ?" << endl;
    } else{
      cout << "File doesn't exist, trying as a folder.." << endl;
      strcat(file->ftpURL, "/*");
      file->isFolder = true;
      download(file);
    }
  } else{
    cout << "Download of " << file->filename << " completed !" << endl;
  }

  free(file);
  pthread_exit(NULL);
};

bool Seedbox::getFilename(char *data, char *buffer){
  char *url = Utils::split(data, ' ', 0)[1];
  char *query = Utils::split(url, '?', 0)[1];
  vector<char*> params = Utils::split(query, '&', 0);

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

bool fileExists(char *path){
  ifstream file(path);
  bool exists = file.good();
  file.close();
  return exists;
};

bool Seedbox::onData(char *data){
  return this->initDownload(data) == 0 ? true : false;
};
