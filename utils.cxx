#include "includes.h"
#include "utils.h"

vector<char *> Utils::split(char *str, char del, int limit){
  vector<char *> splittedStr;

  size_t length = strlen(str);
  size_t count = 0;

  char *buffer = (char*)malloc(length);
  memset(buffer, '\0', length);

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

  free(buffer);

  return splittedStr;
};

#ifdef _WIN32
// Taken from http://stackoverflow.com/questions/10905892/equivalent-of-gettimeday-for-windows
int Utils::gettimeofday(struct timeval * tp, struct timezone * tzp)
{
  // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
  static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

  SYSTEMTIME  system_time;
  FILETIME    file_time;
  uint64_t    time;

  GetSystemTime( &system_time );
  SystemTimeToFileTime( &system_time, &file_time );
  time =  ((uint64_t)file_time.dwLowDateTime )      ;
  time += ((uint64_t)file_time.dwHighDateTime) << 32;

  tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
  tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
  return 0;
}
#endif
