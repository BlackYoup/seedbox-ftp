#include "includes.h"
#include "socket.h"

#ifdef _WIN32
// link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#endif



Socket::Socket(char *port, Seedbox *sb){
  this->sb = sb;
  strcpy(this->port, port);
  this
#ifdef _WIN32
	->initWSA()
#endif
    ->createSocket(port)
    ->bindSocket()
    ->listenConnections()
    ->acceptClients();
};

#ifdef _WIN32
Socket *Socket::initWSA() {
  d.log("Initializing WSA...");

	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		sprintf(this->strError, "WSAStartup failed: %d", iResult);
		this->errorCode = iResult;
	}

  d.log("WSA Initialized");

	return this;
};
#endif

Socket* Socket::createSocket(char *port){
  if(this->errorCode != 0){
    return this;
  }

  memset(&host_info, 0, sizeof host_info);

  host_info.ai_family   = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;
  host_info.ai_flags    = AI_PASSIVE;

  int status = getaddrinfo(
    NULL,
    port,
    &host_info,
    &host_info_list
  );

  if(status != 0){
    sprintf(this->strError, "getaddrinfo error: %s", gai_strerror(status));
    this->errorCode = status;
    return this;
  }

  d.log(DEBUG, "Creating socket...");

#ifdef _WIN32
  socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
  socketfd = socket(
    host_info_list->ai_family,
    host_info_list->ai_socktype,
    host_info_list->ai_protocol
  );
#endif

  if(socketfd == -1){
    sprintf(this->strError, "Error creating socket: %s", strerror(errno));
    this->errorCode = socketfd;
    return this;
  }

  d.log(DEBUG, "Socket created");

  return this;
};

Socket* Socket::bindSocket(){
  if(this->errorCode != 0){
    return this;
  }

  d.log(DEBUG, "Binding socket...");

  // we make use of the setsockopt() function to make sure the port is not in use.
  // by a previous execution of our code. (see man page for more information)

#ifdef _WIN32
  const char yes = '1';
#else
  int yes = 1;
#endif

  int statusSetSockOpt = setsockopt(
    socketfd,
    SOL_SOCKET,
    SO_REUSEADDR,
    &yes,
    sizeof(int)
  );

  if (statusSetSockOpt == -1){
    sprintf(this->strError, "Error setting socket options: %s", strerror(errno));
    this->errorCode = statusSetSockOpt;
    return this;
  }

  int statusBind;

#ifdef _WIN32
  sockaddr_in service;
  service.sin_family = AF_INET;
  service.sin_addr.s_addr = inet_addr("127.0.0.1");
  service.sin_port = htons(stoi(this->port));
  statusBind = bind(
    socketfd,
    (SOCKADDR*)&service,
    sizeof(service)
  );
#else
  statusBind = bind(
    socketfd,
    host_info_list->ai_addr,
    host_info_list->ai_addrlen
  );
#endif

  if(statusBind == -1){
    d.log(DEBUG, "Bad status bind", WSAGetLastError());
    sprintf(this->strError, "Error binding socket: %s", strerror(errno));
    this->errorCode = statusBind;
    return this;
  }

  d.log(DEBUG, "Socket successfuly binded");

  return this;
};

Socket* Socket::listenConnections(){
  if(this->errorCode != 0){
    return this;
  }

  int listenStatus = listen(socketfd, 5);

  if(listenStatus == -1){
    sprintf(this->strError, "Error while listening to connections: %s", strerror(errno));
    this->errorCode = listenStatus;
    return this;
  }

  d.log(DEBUG, "Awaiting connections...");

  return this;
};

Socket* Socket::acceptClients(){
  if(this->errorCode != 0){
    return this;
  }

  struct sockaddr_storage client_info;
  socklen_t client_info_size = sizeof(client_info);

  int client;

  while(true){
    client = accept(Socket::socketfd,
        NULL, NULL);

    struct sockaddr_in *t = (struct sockaddr_in *) &client_info;

    d.log("CLIENT IP", inet_ntoa(t->sin_addr), "PORT", t->sin_port);

    if(client == -1){
      // TODO: handle error
      //sprintf(this->strError, "Error connecting to client: %s", strerror(errno));
      //this->errorCode = client;
    }

    d.log(DEBUG, "Client connected !");
    this->onClient(client);
  }

  return this;
};

void Socket::onClient(int client){
  const int dataLength = 2048;
  char *clientData = (char*)malloc(dataLength);

  this->receiveDataFromClient(client, clientData, dataLength);
  if(this->errorCode != 0){
    this->disconnectClient(client);
    return;
  }

  d.log("Data has been received, calling ondata()");

  if(this->sb->onData(clientData)){
    this->sendBackResponse(client, this->sb->HTTP_RESPONSE_OK);
  } else{
    this->sendBackResponse(client, this->sb->HTTP_RESPONSE_ERR);
  }

  this
    ->disconnectClient(client)
    ->error();
};

Socket *Socket::receiveDataFromClient(int client, char *data, int dataLength){
  if(this->errorCode != 0){
    //return this;
  }

  d.log(DEBUG, "Starting to receive data from client...");

  ssize_t received_bytes = recv(client, data, dataLength, 0);

  if(received_bytes == 0){
    //sprintf(this->strError, "Seems like client closed the connection or didn't send any data");
    //this->errorCode = -1;
    //TODO handle errors;
  } else if(received_bytes == -1){
    //sprintf(this->strError, "Error receiving data from client: %s", strerror(errno));
    //this->errorCode = received_bytes;
    //TODO handle errors;
  }

  d.log("Received from client", data);
  return this;
};

Socket* Socket::sendBackResponse(int client, char *data){
  if(this->errorCode != 0){
    return this;
  }

  Debug d;
  d.log("Sending back", (int)strlen(data) + 1);

  int sendStatus = send(client, data, strlen(data) + 1, 0);

  if(sendStatus == -1){
    d.log(ERR, "Can't send back data to client", strerror(errno));
  }

  d.log(DEBUG, "Sent response to client:", "\n", data);

  return this;
};

Socket* Socket::disconnectClient(int client){
  if(this->errorCode != 0){
    return this;
  }

#ifdef _WIN32
  closesocket(client);
#else
  close(client);
#endif

  d.log(DEBUG, "Client disconnected by server");

  return this;
};

Socket* Socket::error(){
  if(this->errorCode != 0){
    Debug d;
    d.log(ERR, this->strError);
  }
  return this;
};
