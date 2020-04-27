#ifndef HTTPRequest_h
#define HTTPRequest_h

#include<vector>

using namespace std;

class HTTPRequest 
{    
    public:  
        vector<string> getQuery();
        string url;
        vector<string> queryParams;
        HTTPRequest(string request);        
    private:
        vector<string> lines;        
        static bool skipSymbol(char symbol);
        static vector<string> split(string request, char symbol);        
};

#endif