#ifndef __JSON_KEY__
#define __JSON_KEY__
#include <string>
#include <iostream>
#include <json/json.h> 
#include <fstream>
class jsonKey
{
    public:
        static std::string getDownloadToken(const std::string& jsonString);
        static std::string getPrivateKey(const std::string& serviceAccountFile);
};
#endif