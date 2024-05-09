#ifndef __JSON_KEY__
#define __JSON_KEY__
#include <string>
#include <iostream>
#include <json/json.h> 
#include <fstream>
namespace jsonKey
{
    std::string getDownloadToken(const std::string& jsonString);
    std::string getPrivateKey(const std::string& serviceAccountFile);
}
#endif