#include "includes.h"
#include "seedbox.h"
#include "cli.h"
#include "debug.h"

int main(int argc, char *argv[]){
  if(argc < 6){
    Debug d;
    d.log(ERROR, "Missing arguments. You must provide --ftp-user --ftp-password --ftp-host --ftp-base-dir and --save-path");
    return 1;
  }

  CLI cli(argc, argv);

  map<CLI_ARGS,char*> tmp = cli.getArgs();

  Debug d;

  d.log("User:", tmp[ftpUser]);

  new Seedbox((char*)"8080", cli.getArgs());
  return 0;
};
