#include "cloudUrl.h"
std::string cloudUrl::getCURL(ECU ecu, typeURL type,std::string filename, std::string suffixes)
{
    std::string url = "https://firebasestorage.googleapis.com/v0/b/hex-tung-b8ae7.appspot.com/o/";
    if(type == typeURL::METADATA && ecu == ECU::ESP32)
    {
        url += suffixes;
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