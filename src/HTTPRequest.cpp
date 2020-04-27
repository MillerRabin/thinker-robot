#include <Arduino.h>
#include "HTTPRequest.h"

bool HTTPRequest::skipSymbol(char symbol) {
   return (symbol == '\r');
}

vector<string> HTTPRequest::split(string request, char symbol) {
    size_t ln = request.length();       
    size_t sIndex = 0;
    vector<string> pr;
    for (size_t i = 0; i < ln; i++) {
        const char smb = request[i];                
        if (skipSymbol(smb)) continue;
        if (smb == symbol) {
            if (sIndex != i) 
                pr.push_back(request.substr(sIndex, i - sIndex));            
            sIndex = i + 1;        
        }
        if (i == ln - 1) {
            if (sIndex == i) continue;
            pr.push_back(request.substr(sIndex, i - sIndex + 1));            
        }      

    }   
   return pr;
}

HTTPRequest::HTTPRequest(string request) {
    if (request.length() == 0) {        
        return;
    }
    this->lines = split(request, '\n');    
    vector<string> qd = split(this->lines[0], ' ');
    this->url = qd[1];    
    vector<string> qp = split(this->url, '?');
    if (qp.size() > 1)
        this->queryParams = split(qp[1], '&');        
}