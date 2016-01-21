#ifndef SEEDBOX_H
#define SEEDBOX_H

#include "includes.h"
#include "debug.h"
#include "cli.h"

typedef struct {
  char filename[512];
  char fullPath[2048];
  char ftpURL[2048];
  bool isFolder;
  char *savePath;
} fileData;

void *download(void*);
bool fileExists(char*);

class Seedbox{
  private:
    // Methods
    void listend(char *port);
    int initDownload(char*);
    bool getFilename(char*, char*);

    // Attributes
    char savePath[512];
    map<CLI_ARGS,char*> conf;
    Debug d;
  public:
    Seedbox(char*, map<CLI_ARGS,char*>);
    bool onData(char*);

    char HTTP_RESPONSE_OK[256];
    char HTTP_RESPONSE_ERR[256];
};

#endif // SEEDBOX_H
