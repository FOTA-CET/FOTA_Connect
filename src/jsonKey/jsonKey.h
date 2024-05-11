#ifndef __JSON_KEY__
#define __JSON_KEY__
#include <string>
#include <iostream>
#include <json/json.h> 
#include <fstream>
class jsonKey
{
    private:
        static std::string PrivateKey;
        static std::string ProjectID;
        static std::string Token;
    public:
        static std::string getDownloadToken(const std::string& jsonString);
        static void handleFirebaseJson(const std::string& serviceAccountFile);
        static void handleFirebaseToken(const std::string& serviceAccountFile);
        static std::string getProjectID();
        static std::string getPrivateKey();
        static std::string getToken();

};
#endif