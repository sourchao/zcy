#include "util.h"

string GenTimeStamp(const char * format)
{
    time_t now;
    time(&now);
    char timeStampBuff[20];
    strftime(timeStampBuff, sizeof(timeStampBuff), format, localtime(&now));
    string timeStamp = timeStampBuff;
    return timeStamp;
}

/* Class Logger Implement */

Logger & Logger::operator=(const char * str)
{
    *(string *)this = str;
    return *this;
}

Logger & Logger::operator=(string str)
{
    *(string *)this = str;
    return *this;
}

Logger & Logger::operator+=(int num)
{
    char buff[20];
    sprintf(buff, "%d", num);
    *(string *)this += buff;
    return *this;
}

Logger & Logger::operator+=(const char * str)
{
    *(string *)this += str;
    return *this;
}

Logger & Logger::operator+=(string str)
{
    *(string *)this += str;
    return *this;
}

Logger & Logger::operator<<(int num)
{
    char buff[20];
    sprintf(buff, "%d", num);
    *(string *)this += buff;
    return *this;
}

Logger & Logger::operator<<(const char * str)
{
    *(string *)this += str;
    return *this;
}

Logger & Logger::operator<<(string str)
{
    *(string *)this += str;
    return *this;
}
