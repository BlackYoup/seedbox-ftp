#include "includes.h"
#include "utils.h"

using std::vector;

vector<char *> Utils::split(char *str, char del, int limit){
  vector<char *> splittedStr;

  size_t length = strlen(str);
  size_t count = 0;

  char buffer[length] = {'\0'};
  int bufferEntry = 0;
  int found = 0;
  bool limitReached;
  size_t lastElement = length - 1;

  for(; count < length; count++, bufferEntry++){
    bool match = str[count] == del;

    // TODO: too many cpu operations if no limit
    if(limit <= 0){
      limitReached = false;
    } else if(limit > 0){
      limitReached = found < limit;
    }

    if((!limitReached && match) || count == lastElement){
      if(count == lastElement){
        buffer[bufferEntry] = str[count];
        buffer[bufferEntry+1] = '\0';
      }

      // TODO: need to free these buffers
      char *x = (char*)malloc(strlen(buffer) + 1);
      std::strcpy(x, buffer);

      splittedStr.push_back(x);

      bufferEntry = -1;
      memset(buffer, '\0', length);
    } else{
      if(match && !limitReached){
        found++;
      }
      buffer[bufferEntry] = str[count];
    }
  }

  return splittedStr;
};

char *Utils::strcpy(char *dest, char *source, int sourceStart, int sourceEnd){
  if(sourceEnd <= 0 || sourceStart > sourceEnd){
    sourceEnd = strlen(source);
  }

  char buffer[sourceEnd - sourceStart + 1];

  for(int i = 0; sourceStart < sourceEnd; i++, sourceStart++){
    buffer[i] = source[sourceStart];
  }

  buffer[sourceEnd] = '\0';

  return std::strcpy(dest, buffer);
};
