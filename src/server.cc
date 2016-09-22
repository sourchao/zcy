#include "server.h"

Server::Server(int listen_port, string wavDir)
{
    signal(SIGCHLD, SIG_IGN);
    fstream fs(wavDir, ios::in);
    if (!fs)
        throw string("ERROR [ Parameter ]: " + wavDir + " is not exist");
        
    if (wavDir.rfind('/') + 1 != wavDir.size())
        wavDir += '/';
    _wavDir = wavDir;
    
    if(( _socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ) 
        throw string("ERROR [ Socket ]: Socket failure");    
    int reuseaddr = 1;
    setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
  
    memset(&_server_addr, 0, sizeof(_server_addr));//has sizeof,the head of myserver will be 0;
    _server_addr.sin_family = AF_INET;  
    _server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    _server_addr.sin_port = htons(listen_port);  //设置的端口为listen_port
    
    if( bind(_socket_fd, (sockaddr *)&_server_addr, sizeof(_server_addr)) < 0 )  
        throw string("ERROR [ Socket ]: Bind failure"); 
    
    int maxClientNum = 16;
    if( listen(_socket_fd, maxClientNum) < 0 ) 
        throw string("ERROR [ Socket ]: Listen failure");  
    
    cout << genServerTimeStamp() << " Server started, listen in [ " << listen_port << " ]" << endl;
    cout << "    Accept [ " << maxClientNum << " ] active clients at the same time" << endl;
}  
  
int Server::Start() 
{  
    while( 1 ) 
    {  
        socklen_t sin_size = sizeof(struct sockaddr_in);  
        if(( _accept_fd = accept(_socket_fd, (struct sockaddr *)&_remote_addr, &sin_size)) == -1 )  
        {  
            throw string("ERROR [ Socket ]: Accept failure, NO more clients can be connect");  
            continue;  
        }
        string clientIP = inet_ntoa(_remote_addr.sin_addr);
        int clientPort  = ntohs(_remote_addr.sin_port);
        string serverTimeStamp = genServerTimeStamp();
        string fileTimeStamp = genFileTimeStamp();
        cout << serverTimeStamp << " Incomming request from [ " << clientIP << ":" \
                                                                    << clientPort << " ]" << endl;
	    int pid = fork();
        if( !pid ) 
        {
            signal(SIGPIPE, SIG_IGN);
            _summary << "    INFO: Connection start time = [ " << serverTimeStamp << " ]\n";
            _summary << "    INFO: Client socket = [ " << clientIP << ":" << clientPort << " ]\n";
            /*
             * Authentication data format: 
             *      username password docid depid port\n
             * Sample: 
             *      10000011 123456 10000011 CHK 5000\n
             */
            // 验证从客户端发来的身份数据
            bool isIdentityValid = authIdentity(5);
            _summary << "    INFO: Authentication data = [ " << _bIdentityData << " ]\n";
            if (!isIdentityValid)
                throw string("    ERROR: Authenticating identity failed");
            
            SRManager srMgr;
            if (!srMgr.Login(NULL, NULL))
                throw string("    ERROR: Login to iflytek API failed");
            
            if (!srMgr.BeginSession())
                throw string("    ERROR: Create session failed");

            WavStream<Byte> wavStream;
            WavReader<Byte> wavReader;
            wavReader.SetStream(&wavStream);
            wavReader.SetSource(_accept_fd);
            // 开启新的线程接收客户端发来的wav数据
            if (!wavReader.BeginRead())
                throw string("    ERROR: Invalid stream or source");

            int chunk_size = 6400; // 200ms wav
            Byte *chunk = new Byte[chunk_size];
            int len = 0;
            while (srMgr.IsSessionValid()) {
                len = wavReader.Read(chunk, chunk_size);
                if (len == 0)
                    break;
                // 将读取的一块wav数据发送至科大讯飞API
                if (!srMgr.SendData(chunk, len))
                    throw string("    ERROR: Send wav data failed");
                srMgr.AskResult();
            }
            wavReader.EndRead();
            
            // 输出音频长度
            double wavLength = wavReader.GetWavDataLength() * 1.0 / 32000;
            char szWavLength[10];
            sprintf(szWavLength, "%.2lf", wavLength);
            _summary << "    INFO: Wav length = [ " << szWavLength << " ] secs\n";

            srMgr.SendFinish();
            // 阻塞等待科大讯飞API发回完整的识别结果
            srMgr.WaitAllResults();
            srMgr.EndSession();
            srMgr.Logout();
            
            // 将最终识别结果发给客户端
            string sr_result = srMgr.GetResult();
            _summary << "    INFO: Recognition result = [ " << sr_result << " ]\n";
            if (send(_accept_fd, sr_result.c_str(), sr_result.length(), 0) == -1)
                throw string("    ERROR: Connection reset by peer.");
            
            // 获取不带后缀的文件名(name, not full name)
            string fileName = genFileName(fileTimeStamp);
            // 设置wav header信息，并将wav header与data写入wav文件生成音频
            string wavFilePath = _wavDir + fileName + ".wav";
            WavWriter<Byte> wavWriter;
            wavWriter.SetFilePath(wavFilePath);
            wavWriter.SetStream(&wavStream);
            wavWriter.SetSampleFrequency(16000);
            wavWriter.SetChannelNum(1);
            wavWriter.SetSampleWidth(2);
            wavWriter.WriteAll();
            wavWriter.Close();
            _summary << "    INFO: Wav file save as [ " << wavFilePath << " ]\n";
            
            // 将最终识别结果处理成可用于训练的结果，保存为txt文件
            string txtFilePath = _wavDir + fileName + ".txt";
            ofstream txtFile(txtFilePath, ios::out);
            txtFile << fileName << "\t" << genTrainingText(sr_result);
            txtFile.close();
            _summary << "    INFO: Text file save as [ " << txtFilePath << " ]\n";

            _summary << "    DONE!\n\n";
            LogTo(cout);
            close(_accept_fd);
            exit(0);
         }
         close(_accept_fd);
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
        
    int nRecvParamCnt = 1;
    for (int i = 0; i < len; i++) {
        if (_bIdentityData[i] == ' ')
            nRecvParamCnt++;
    }
    
    if (nRecvParamCnt != nNeedParamCnt)
        return false;
    return true;
}

string Server::genFileName(string timeStamp)
{
	stringstream stringStream(_bIdentityData);
	string token[5];
	stringStream >> token[0] >> token[1] >> token[2] >> token[3] >> token[4];
    string filename = token[2] + "_" + token[3] + "_" + token[4] + "_" + timeStamp;
    return filename;
}

string Server::genTrainingText(string result)
{
    return result;
}

string Server::genServerTimeStamp()
{
    return GenTimeStamp("%Y-%m-%d %H:%M:%S");
}

string Server::genFileTimeStamp()
{
    return GenTimeStamp("%Y-%m-%d-%H-%M-%S");
}

void Server::LogTo(ostream & out)
{
    out << _summary;
}
