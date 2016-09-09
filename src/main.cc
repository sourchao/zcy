#include <iostream>  
#include "Server.h"  
  
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
