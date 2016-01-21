#ifndef HTTP_SOCKET_H
#define HTTP_SOCKET_H

#include "includes.h"
#include "debug.h"
#include "seedbox.h"

class Socket{
  private:
    int socketfd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    Debug d;

    Socket* bindSocket();
    Socket* createSocket(char*);
    Socket* listenConnections();
    Socket* acceptClients();
    Socket* sendBackResponse(int, char*);
    Socket* disconnectClient(int);
    Socket* error();
    Socket* receiveDataFromClient(int, char*, int);
    void onClient(int);

    int errorCode = 0;
    char strError[128];
    Seedbox *sb;
  public:
    Socket(char*, Seedbox*);
};

#endif
