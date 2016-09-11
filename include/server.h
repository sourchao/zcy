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
#include <iomanip>
#include <fstream>
#include "sr_mgr.h"
#include "types.h"
#include "wav.h"
#include "util.h"

using namespace std;
  
#define BUFFER_SIZE 1024
  
class Server
{  
public:  
    Server(int listen_port);  
    int Start();
    
private:
    bool authIdentity(int nNeedParamCnt);
    string genFileName();
    string genTrainingText(string result);

    int _socket_fd, _accept_fd;  
    sockaddr_in _server_addr;  
    sockaddr_in _remote_addr;
    string _bIdentityData;
    string _summary;
}; 
#endif
