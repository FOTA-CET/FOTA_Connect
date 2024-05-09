#ifndef __CLOUD_URL__
#define __CLOUD_URL__
#include <string>
namespace cloudUrl
{
    enum class ECU {ESP32, ATMEGA328P, STM32};
    enum class typeURL{METADATA, DATA};
    std::string getCURL(ECU ecu, typeURL type,std::string filename="", std::string suffixes = ""); // use default parameter
}
#endif // include guard