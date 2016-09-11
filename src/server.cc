#include "server.h"

Server::Server(int listen_port) 
{  
    if(( _socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
    {  
        throw string("socket failed");  
    }  
  
    memset(&_server_addr, 0, sizeof(_server_addr));//has sizeof,the head of myserver will be 0;
    _server_addr.sin_family = AF_INET;  
    _server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    _server_addr.sin_port = htons(listen_port);  //设置的端口为listen_port
    
    if( bind(_socket_fd, (sockaddr *)&_server_addr, sizeof(_server_addr)) < 0 ) 
    {  
        throw string("bind failed");
    }  
    
    if( listen(_socket_fd, 10) < 0 )
    {  
        throw string("listen failed");  
    }  
    cout << "Server started." << endl;
}  
  
int Server::Start() 
{  
    while( 1 ) 
    {  
        socklen_t sin_size = sizeof(struct sockaddr_in);  
        if(( _accept_fd = accept(_socket_fd, (struct sockaddr *)&_remote_addr, &sin_size)) == -1 )  
        {  
            throw string("Accept error!");  
            continue;  
        }  
        cout << "Received a connection from " << inet_ntoa(_remote_addr.sin_addr) << endl;
 
	    int pid = fork();
        if( !pid ) 
        {
            /*
             * Authentication data format: 
             *      username password docid depid port\n
             * Sample: 
             *      10000011 123456 10000011 CHK 5000\n
             */
            // 验证从客户端发来的身份数据
            if (!authIdentity(5))
                throw string("Error: Authenticating identity failed.");
            
            SRManager srMgr;
            if (!srMgr.Login(NULL, NULL))
                throw string("Login to iflytek API failed");
            
            WavStream<Byte> wavStream;
            WavReader<Byte> wavReader;
            wavReader.SetStream(&wavStream);
            wavReader.SetSource(_accept_fd);
            // 开启新的线程接收客户端发来的wav数据
            wavReader.BeginRead();  // pthread

            int chunk_size = 6400; // 200ms wav
            Byte *chunk = new Byte[chunk_size];
            int len = 0;
            while (len = wavReader.Read(chunk, chunk_size)) {
                // 将读取的一块wav数据发送至科大讯飞API
                if (!srMgr.SendData(chunk, len))
                    throw string("  ERROR: Send wav data failed");
            }
            
            // 阻塞等待科大讯飞API发回完整的识别结果
            string sr_result = srMgr.GetResult();
            // 将最终识别结果发给客户端
            if (send(_accept_fd, sr_result.c_str(), sr_result.length(), 0) == -1) {
                throw string("Error: Connection reset by peer.");
            }
            /*
            // TODO:: 需要先获取存放目录，可通过解析命令行参数或读取配置文件的方式获得。
            string wavDir;
            // 获取不带后缀的文件名(name, not full name)
            string fileName = genFileName();
            // 设置wav header信息，并将wav header与data写入wav文件生成音频
            string wavFileName = wavDir + fileName + ".wav";
            WavWriter<Byte> wavWriter;
            wavWriter.SetFilePath(wavFilePath);
            wavWriter.SetStream(&wavStream);
            wavWriter.SetSampleFrequency(16000);
            wavWriter.SetChannelNum(1);
            wavWriter.SetSampleWidth(2);
            wavWriter.Write();
            wavWriter.Close();
            
            // 将最终识别结果处理成可用于训练的结果，保存为txt文件
            string txtFilePath = wavDir + fileName + ".txt";
            ofstream txtFile(txtFilePath, ios::out);
            txtFile << fileName << "\t" << genTrainingText(sr_result) << endl;
            txtFile.close();
            */
            close(accept_fd);
            exit(0);
         }
         close(accept_fd);
    }  
        return 0;  
}

bool Server::authIdentity(int nNeedParamCnt)
{
    Byte buff[BUFFER_SIZE];
    int len = read(_accept_fd, buff, sizeof(buff));
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

string Server::genFileName()
{
	istringstream stringStream(_bIdentityData);
	string token[5];
	stringStream >> token[0] >> token[1] >> token[2] >> token[3] >> token[4];
    string timeStamp = GenTimeStamp("%Y-%m-%d-%H-%M-%S");
    string wavFilename = token[2] + "_" + token[3] + "_" + token[4] + "_" + timeStamp;
    return wavFilename;
}
