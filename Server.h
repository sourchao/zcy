#include <unistd.h>  
#include <iostream>  
#include <sys/socket.h>  
#include <arpa/inet.h>  
#include <netinet/in.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;
  
#define MAXSIZE 2000  
#define AUTHENSIZE 1024
  
class server  
{  
private:
  
    int socket_fd,accept_fd;  
    sockaddr_in myserver;  
    sockaddr_in remote_addr;  
  
public:  
    server(int listen_port);  
    int start();  
}; 
string constructFileName(char* temp);
void authenInfo(char* temp);
//string readLine(int socket_fd,char* buffer,int MAXSIZE){}
