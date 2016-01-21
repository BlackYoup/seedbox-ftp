#ifndef CLI_H
#define CLI_H

#include "includes.h"
#include "debug.h"

typedef enum{
  ftpUser,
  ftpPassword,
  ftpHost,
  ftpBaseDir,
  savePath
} CLI_ARGS;

class CLI{
  private:
    void parseArgs(int, char**);

    map<CLI_ARGS,char*> args;
    Debug d;
  public:
    CLI(int, char**);
    map<CLI_ARGS,char*> getArgs();
};

#endif // CLI_H
