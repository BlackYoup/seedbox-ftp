#include "debug.h"

#ifdef _WIN32
#include "utils.h"
#endif

char* Debug::getTime(){
  timeval now;
#ifdef _WIN32
  Utils::gettimeofday(&now, NULL);
#else
  gettimeofday(&now, NULL);
#endif
  int milliseconds = now.tv_usec / 1000;

  char buffer[Debug::sizeOfStrTime];
  char *tempDate = new char[Debug::sizeOfStrTime];

#ifdef _WIN32
  time_t x = now.tv_sec;
  strftime(buffer, sizeOfStrTime, "%H:%M:%S", localtime(&x));
#else
  strftime(buffer, sizeOfStrTime, "%H:%M:%S", localtime(&now.tv_sec));
#endif

  sprintf(tempDate, "%s.%d", buffer, milliseconds);

  return tempDate;
};

void Debug::printHeader(DEBUG_LEVEL level) {
#ifdef _WIN32
  printWindowsHeader(level);
#else
  printUNIXHeader(level);
#endif
}

#ifndef _WIN32
// TODO: pass a buffer instead of returning a char
char const* Debug::getUNIXLogLevelStr(DEBUG_LEVEL level){
  char const* level_str;

  switch(level){
    case ERR:
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
};

void Debug::printUNIXHeader(DEBUG_LEVEL level){
  std::cout
    << "\033[1;33m["
    << getTime()
    << "]\033[0m"
    << getUNIXLogLevelStr(level);
};
#else
// TODO: get ride of system() call: http://www.cplusplus.com/forum/beginner/5830/
void Debug::printWindowsHeader(DEBUG_LEVEL level) {
  std::cout << getTime() << " ";

  switch (level) {
  case ERR:
    system("Color 0C");
    std::cout << "[ERROR] ";
    break;
  case INFO:
    system("Color 0A");
    std::cout << "[INFO] ";
    break;
  case DEBUG:
    system("Color 0B");
    std::cout << "[DEBUG] ";
    break;
  }
};
#endif