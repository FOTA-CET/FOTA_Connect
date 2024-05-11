#include "cloudUrl.h"
#include "jsonKey.h"
std::string cloudUrl::getCURL(ECU ecu, typeURL type,std::string filename, std::string suffixes)
{
    std::string url = "https://firebasestorage.googleapis.com/v0/b/";
    url += jsonKey::getProjectID();
    url += ".appspot.com/o/";
    url += enumToDownloadString(ecu);
    if(type == typeURL::METADATA && ecu == ECU::ESP32)
    {
        url += filename;
        url += ".bin";
        return url;
    }
    else if(type == typeURL::METADATA && ecu != ECU::ESP32)
    {
        url += filename;
        url += ".hex";
        return url;
    }
    else if(type == typeURL::DATA && ecu != ECU::ESP32)
    {
        url += filename;
        url += ".hex?alt=media&token=";
        url += suffixes;
        return url;
    }
    else
    {
        url += filename;
        url += ".bin?alt=media&token=";
        url += suffixes;
        return url;
    }
}

std::string cloudUrl::enumToDownloadString(ECU ecu)
{
    if(ecu == ECU::ESP32) return "ESP32%2F";
    else if(ecu == ECU::ATMEGA328P) return "ATMEGA328P%2F";
    else return "STM32%2F";
}