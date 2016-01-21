#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include "debug.h"


char* Debug::getTime(){
  timeval now;
  gettimeofday(&now, NULL);
  int milliseconds = now.tv_usec / 1000;

  char buffer[Debug::sizeOfStrTime];
  char *tempDate = new char[Debug::sizeOfStrTime];

  strftime(buffer, sizeOfStrTime, "%H:%M:%S", localtime(&now.tv_sec));
  sprintf(tempDate, "%s.%d", buffer, milliseconds);

  return tempDate;
};

char const* Debug::getLogLevelStr(DEBUG_LEVEL level){
  char const* level_str;

  switch(level){
    case ERROR:
      level_str = "\033[1;31m[ERROR]\033[0m ";
    break;
    case INFO:
      level_str = "\033[1;32m[INFO]\033[0m ";
    break;
    case DEBUG:
      level_str = "\033[1;36m[DEBUG]\033[0m ";
    break;
    default:
      level_str = "";
    break;
  }

  return level_str;
}

void Debug::printHeader(DEBUG_LEVEL level){
  std::cout
    << "\033[1;33m["
    << getTime()
    << "]\033[0m"
    << getLogLevelStr(level);
};
