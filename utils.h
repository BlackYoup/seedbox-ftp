#ifndef UTILS_H
#define UTILS_H

#include "includes.h"

class Utils{
  public:
    static std::vector<char*> split(char*, char, int);
    static int gettimeofday(struct timeval*, struct timezone*);
};

#endif
