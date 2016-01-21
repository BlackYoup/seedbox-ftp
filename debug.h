#pragma once
#include <string.h>
#include <iostream>
#include <typeinfo>

enum DEBUG_LEVEL{
  ERROR = 1,
  INFO  = 2,
  DEBUG = 3
};

class Debug{
  private:
    char* getTime();
    char const* getLogLevelStr(DEBUG_LEVEL);
    void printHeader(DEBUG_LEVEL);
    template <typename T, typename... Args> void printBody(T, Args...);
    template <typename T> void printBody(T);

    const int sizeOfStrTime = sizeof "00:00:00.000";
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
