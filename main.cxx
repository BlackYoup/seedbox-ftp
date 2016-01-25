#include "includes.h"
#include "seedbox.h"
#include "cli.h"
#include "debug.h"

int main(int argc, char *argv[]){
  if(argc < 6){
    Debug d;
    d.log(ERR, "Missing arguments. You must provide --ftp-user --ftp-password --ftp-host --ftp-base-dir and --save-path");
#ifdef _WIN32
    system("pause");
#endif
    return 1;
  }

  CLI cli(argc, argv);
  new Seedbox((char*)"8080", cli.getArgs());
#ifdef _WIN32
  system("pause");
#endif
  return 0;
};
