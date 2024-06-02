#ifndef __CLOUD_URL__
#define __CLOUD_URL__
#include <string>
#include <iostream>
enum class ECU {ESP32, ATMEGA328P, STM32};
enum class typeURL{METADATA, DATA};
enum class ECU_Status{NONE, UPDATE, REJECT, FAILED};
class cloudUrl
{
    private:
        static std::string enumToDownloadString(ECU ecu);
    public:
        static std::string getCURL(ECU ecu, typeURL type,std::string filename="", std::string suffixes = ""); // use default parameter
};
#endif // include guard