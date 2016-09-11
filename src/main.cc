#include "server.h"  
  
int main(int argc, char* argv[])  
{  
    try
    {
        Server ts(atoi(argv[1]));
        ts.Start();
    }    
    catch(string ex)
    {
        cout << ex << endl;
    } 
    
    return 0;  
}   
