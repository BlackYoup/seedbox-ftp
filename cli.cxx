#include "cli.h"
#include "utils.h"

CLI::CLI(int argc, char *argv[]){
  this->parseArgs(argc, argv);
};

void CLI::parseArgs(int argc, char *argv[]){
  for(int i = 0; i < argc; i++){
    vector<char*> tmp = Utils::split(argv[i], '=', 0);

    if(strcmp(tmp[0], "--ftp-user") == 0){
      d.log("Got FTP-USER:", tmp[1], "PARAM:", tmp[0]);
      this->args.insert(pair<CLI_ARGS,char*>(ftpUser, tmp[1]));
    } else if(strcmp(tmp[0], "--ftp-password") == 0){
      d.log("Got FTP-PASSWD:", tmp[1]);
      this->args.insert(pair<CLI_ARGS,char*>(ftpPassword, tmp[1]));
    } else if(strcmp(tmp[0], "--ftp-host") == 0){
      d.log("Got FTP-HOST:", tmp[1]);
      this->args.insert(pair<CLI_ARGS,char*>(ftpHost, tmp[1]));
    } else if(strcmp(tmp[0], "--ftp-base-dir") == 0){
      d.log("Got FTP-BASE-DIR", tmp[1]);
      this->args.insert(pair<CLI_ARGS,char*>(ftpBaseDir, tmp[1]));
    } else if(strcmp(tmp[0], "--save-path") == 0){
      d.log("Got SAVE-PATH:", tmp[1]);
      this->args.insert(pair<CLI_ARGS,char*>(savePath, tmp[1]));
    }
  }
};

map<CLI_ARGS, char*> CLI::getArgs(){
  return this->args;
};
