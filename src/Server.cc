#include "Server.h"

server::server(int listen_port) 
{  
    if(( socket_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0 )
    {  
        throw string("socket failed");  
    }  
  
    memset(&myserver,0,sizeof(myserver));//has sizeof,the head of myserver will be 0;
    myserver.sin_family = AF_INET;  
    myserver.sin_addr.s_addr = htonl(INADDR_ANY);  //IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    myserver.sin_port = htons(listen_port);  //设置的端口为listen_port
    
    if( bind(socket_fd,(sockaddr*) &myserver,sizeof(myserver)) < 0 ) 
    {  
        throw string("bind failed");
    }  
    
    if( listen(socket_fd,10) < 0 )
    {  
        throw string("listen failed");  
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
            throw string("Accept error!");  
            continue;  
        }  
        cout<<"Received a connection from "<<(char*) inet_ntoa(remote_addr.sin_addr)<<endl;
 
	    int pid = fork();
        if( !pid ) 
        {
            /*
             * Authentication data format: 
             *      username password docid depid port\n
             * Sample: 
             *      10000011 123456 10000011 CHK 5000\n
             */
            if (!authIdentity(5))
                throw string("Error: Authenticating identity failed.");
            
            SRManager srMgr;
            if (!srMgr.Login(NULL, NULL))
                throw string("Login to iflytek API failed");
            
            WavStream<Byte> *wavStream = new WavStream<Byte>();
            wavStream.SetPeer(accept_fd);
            wavStream.BeginRead();

            int chunck_size = 6400; // 200ms wav
            Byte *chunck = new Byte[chunck_size];
            while (wavStream.Read(chunck, chunck_size)) {
                srMgr.SendData(chunck, chunck_size);
            }
            
            string sr_result = srMgr.GetResult();
            if (send(accept_fd, sr_result.c_str(), sr_result.length(), 0) == -1) {
                throw string("Error: Connection reset by peer.");
            }
         }
         close(accept_fd);
    }  
        return 0;  
}

bool server::authIdentity(int nNeedParamCnt)
{
    Byte buff[BUFFER_SIZE];
    int len = read(accept_fd, buff, sizeof(buff));
    if (len == -1 || buff[len - 1] != '\n')
        return false;
    
    buff[len - 1] = '\0';   // replace '\n' to '\0'
    _bIdentityData = (char *)buff;
#ifdef DEBUG    
    cout << "Receiving authentication data: " << _bIdentityData << endl;
#endif
    int nRecvParamCnt = 1;
    for (int i = 0; i < len; i++) {
        if (_bIdentityData[i] == ' ')
            nRecvParamCnt++;
    }
    
    if (nRecvParamCnt != nNeedParamCnt)
        return false;
    return true;
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
