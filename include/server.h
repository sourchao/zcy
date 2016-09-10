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
#include <iomanip.h>
#include <fstream.h>
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
    string genWavFileName();
    string genTrainingText(string result);

    int _socket_fd, _accept_fd;  
    sockaddr_in _server_addr;  
    sockaddr_in _remote_addr;
    string _bIdentityData; 
}; 
string constructFileName(char* temp);
//string readLine(int socket_fd,char* buffer,int MAXSIZE){}
#endif
