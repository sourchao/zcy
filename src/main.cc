#include <iostream>  
#include "Server.h"  

class Exception {
private :
    int errno;
public :
    Exception(int code) { errno = code; }
    int getErrno() { return errno; }
};

  
int main(int argc,char* argv[])  
{  
    try
    {
        server ts(atoi(argv[1]));
        ts.start();
    }    
    catch(string ex)
    {
        cout<<ex<<endl;
    } 
    return 0;  
}   
