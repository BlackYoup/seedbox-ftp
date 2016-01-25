#ifndef DEBUG_H
#define DEBUG_H

#include "includes.h"

enum DEBUG_LEVEL{
  ERR = 1,
  INFO  = 2,
  DEBUG = 3
};

class Debug{
  private:
    char* getTime();

#ifndef  _WIN32
    char const* getUNIXLogLevelStr(DEBUG_LEVEL);
    void printUNIXHeader(DEBUG_LEVEL);
#else
    void printWindowsHeader(DEBUG_LEVEL);
#endif

    void printHeader(DEBUG_LEVEL);

    template <typename T, typename... Args> void printBody(T, Args...);
    template <typename T> void printBody(T);

    static const int sizeOfStrTime = 10;
  public:
    template <typename T, typename... Args> void log(DEBUG_LEVEL, T, Args...);
    template <typename T> void log(DEBUG_LEVEL, T);
    template <typename T, typename... Args> void log(T, Args...);
};

template <typename T, typename... Args>
void Debug::log(DEBUG_LEVEL level, T message, Args... args){
  printHeader(level);
  printBody(message, args...);
};

template <typename T>
void Debug::log(DEBUG_LEVEL level, T message){
  printHeader(level);
  printBody(message);
};

template<typename T, typename... Args>
void Debug::log(T message, Args... args){
  log(DEBUG, message, args...);
};

template <typename T, typename... Args>
void Debug::printBody(T message, Args... args){
  std::cout << message;
  bool isLineReturn = false;

  if(strcmp(typeid(message).name(), typeid(char const*).name()) == 0){
    char const* test = (char const*)message;
    if(strcmp(test, "\n") == 0){
      isLineReturn = true;
    }
  }

  if(!isLineReturn){
    std::cout << " ";
  }

  printBody(args...);
};

template <typename T>
void Debug::printBody(T message){
  std::cout << message << std::endl;
};
#endif