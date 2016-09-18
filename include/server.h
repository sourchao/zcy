#ifndef _SERVER_H_
#define _SERVER_H_
   
#include <sys/socket.h>  
#include <sys/types.h>
#include <arpa/inet.h>  
#include <netinet/in.h>
#include <iomanip>
#include "sr_mgr.h"
#include "types.h"
#include "wav.h"
#include "util.h"
#include "common.h"
  
#define BUFFER_SIZE 1024
  
class Server
{  
public:  
    Server(int listen_port, string wavDir); 
    int Start();
    void LogTo(ostream & out);
    
private:
    bool authIdentity(int nNeedParamCnt);
    string genFileName(string timeStamp);
    string genTrainingText(string result);
    string genServerTimeStamp();
    string genFileTimeStamp();

    int _socket_fd, _accept_fd;  
    sockaddr_in _server_addr;  
    sockaddr_in _remote_addr;
    string _bIdentityData;
    Logger _summary;
    string _wavDir;
}; 
#endif
