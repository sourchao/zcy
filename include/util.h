#ifndef _COMMON_UTIL_H_
#define _COMMON_UTIL_H_

#include <time.h>
#include "common.h"

string GenTimeStamp(const char * format);

class Logger : public string {
public:
    Logger & operator=(string str);
    Logger & operator=(const char * str);
    Logger & operator+=(string str);
    Logger & operator+=(const char * str);
    Logger & operator+=(int num);
    Logger & operator<<(string str);
    Logger & operator<<(const char * str);
    Logger & operator<<(int num);
};

#endif
