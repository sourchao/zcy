#include <iostream>  
#include "Server.h"  
  
int main(int argc,char* argv[])  
{  
    server ts(atoi(argv[1]));  
    
    try
    {
        ts.start();
    }    
    catch(string ex)
    {
        cout<<ex<<endl;
    } 
    return 0;  
}   
