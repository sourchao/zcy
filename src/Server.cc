#include "Server.h"
#include <time.h>
#include <vector>
#include <sstream>
  
server::server(int listen_port) 
{  
    if(( socket_fd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0 )
    {  
        throw "socket() failed";  
    }  
  
    memset(&myserver,0,sizeof(myserver));//has sizeof,the head of myserver will be 0;
    myserver.sin_family = AF_INET;  
    myserver.sin_addr.s_addr = htonl(INADDR_ANY);  //IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    myserver.sin_port = htons(listen_port);  //设置的端口为listen_port
    if( bind(socket_fd,(sockaddr*) &myserver,sizeof(myserver)) < 0 ) 
    {  
        throw "bind() failed";  
    }  
    if( listen(socket_fd,10) < 0 )
    {  
        throw "listen() failed";  
    }  
    cout<<"=====waiting for client's request====="<<endl;
}  
  
int server::start() 
{  
    while( 1 ) 
    {  
        socklen_t sin_size = sizeof(struct sockaddr_in);  
        if(( accept_fd = accept(socket_fd,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )  
        {  
            throw "Accept error!";  
            continue;  
        }  
        cout<<"Received a connection from "<<(char*) inet_ntoa(remote_addr.sin_addr)<<endl;
 
	    int pid = fork();
        if( !pid ) 
        { 
            //vector<char> buff;
            
            char buffer[AUTHENSIZE];  
            memset(buffer,0,AUTHENSIZE);  
            if( ( read(accept_fd,buffer,AUTHENSIZE)) < 0 ) 
            {  
                throw("Read() error!");  
            } 

		    authenInfo(buffer);
		    string szFileName = constructFileName(buffer);
            bool flag = true;
            char buff[MAXSIZE];
            vector<char> buffVector;
            while(flag)
            {
	            memset(buff,0,MAXSIZE);
		        int num = read(accept_fd,buff,MAXSIZE);
		        if( num  < 0 ) 
                {  
                    throw("Read() error!");  
                } 
		        else
		        {
		            int lastIntValue = *(int*)(&buff + num -4);
		        }
		        int j = 0;
		        while(buff[j] != '\n')
		        {
		            buffVector.push_back(buff[j]);
		            j++;
		        }
		      
	        }
	        exit(0);
            /*else 
            {  
                cout<<"Received message: "<<buffer<<endl;  
                break;  
            }  
            exit(0);  
            */
	        /*
            getAuthenticationData()  // blocking
            while (true) {
                if (finished())
                    break;
                getWavData()  // blocking
                //if (hasEnouthData())
                //    callIflytekAPI();
            }
            //if (hasRemainData())
            //    callIflytekAPI();
            */
         }
         close(accept_fd);
    }  
        return 0;  
}
void authenInfo(char* temp)
{
    int i = 1;
    while(*temp != '\n')
    {
	if(*temp == ' ')
	    i++;
	temp++;
    }
    if(i == 5)
        cout<<"Authentication is passed"<<endl;
    else
    {
        cout<<"Authentication is not passed"<<endl;
        exit(-1);
    }
}
string constructFileName(char* temp)
{
    int i=0;
    vector<char> bufferByte;
    while(*temp != '\n')
	{
		bufferByte.push_back(*temp);
		temp++;
	}
	string firstLine(&bufferByte.front(),bufferByte.size());
	istringstream is(firstLine);
	string s1,s2,s3,s4,s5;
	is >> s1 >> s2 >> s3 >> s4 >> s5;
    time_t now;
    time(&now);
    char timeStamp[20];
    strftime(timeStamp, sizeof(timeStamp), "%Y-%m-%d-%H-%M-%S", localtime(&now));
    string szFilename = s3 + "_" + s4 + "_" + s5 + "_" + timeStamp + ".wav";
    return szFilename;
}
