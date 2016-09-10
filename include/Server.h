#ifndef _SERVER_H_
#define _SERVER_H_

#include <unistd.h>  
#include <iostream>  
#include <sys/socket.h>  
#include <arpa/inet.h>  
#include <netinet/in.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <time.h>
#include <vector>
#include "sr_mgr.h"
#include "types.h"

using namespace std;
  
#define BUFFER_SIZE 1024
  
class server
{  
public:  
    server(int listen_port);  
    int start(); 
    
private:
    bool authIdentity(int nNeedParamCnt);

    int socket_fd,accept_fd;  
    sockaddr_in myserver;  
    sockaddr_in remote_addr;
    string _bIdentityData; 
}; 
string constructFileName(char* temp);
//string readLine(int socket_fd,char* buffer,int MAXSIZE){}
#endif
