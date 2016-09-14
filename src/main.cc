#include "server.h"  
  
int main(int argc, char* argv[])  
{
  
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " port wavDir" << endl;
        return -1;
    }
    
    bool isServerStarted = false;
    Server * server;
    try
    {
        server = new Server(atoi(argv[1]), argv[2]);
        isServerStarted = true;
        server->Start();
    }    
    catch(string ex)
    {
        if (isServerStarted)
            server->LogTo(cout);
        cout << ex << endl << endl;
    } 
     
    return 0;  
}   
