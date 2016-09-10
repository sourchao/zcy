#include "util.h"

char * GenTimeStamp(const char * format)
{
    time_t now;
    time(&now);
    char timeStamp[20];
    strftime(timeStamp, sizeof(timeStamp), format, localtime(&now));
    return timeStamp;
}
