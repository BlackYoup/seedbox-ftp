// All includes

#ifdef linux
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/time.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <Windows.h>
#include <stdint.h> // portable: uint64_t   MSVC: __int64
#include <time.h>
#endif

#include <sys/types.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <typeinfo>
#include <string>

using std::vector;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::ifstream;
using std::sprintf;
using std::stoi;

#ifdef _WIN32
#ifdef _WIN64
typedef unsigned __int64 ssize_t;
#else
typedef unsigned int ssize_t;
#endif
#endif
