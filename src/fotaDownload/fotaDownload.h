#ifndef __FOTA_DOWNLOAD_
#define __FOTA_DOWNLOAD_

#include <iostream>

#include "cloudUrl.h"
#include "restAdapter.h"
#include "jsonKey.h"
#include "cloudUrl.h"
#include <json/json.h> 

enum class Status{OK, ERROR};
class fotaDownload
{
    public:
        static std::string firmwareMetadataDir;
        static std::string getECUName(std::string& nameFirmware);
        static std::string getFirmwareVersion(std::string& nameFirmware);
        static Status getNameFirmware(std::string& nameFirmware);
        static Status getFactoryResetName(std::string& factoryResetName);
        Status download(ECU ecu, std::string& fileName, std::string& path);
        static bool updateFirmwareList(std::string& newName);
        static void setfirmwareMetadata(std::string& firmwareMetadata);
        static bool resetUpdateFieldFirebase();
        static bool resetFactoryResetFieldFirebase();
        static bool updateMCUStatus(std::string& ecu, std::string&& status);
        static bool updatePercent(std::string& ecu, std::string& percent);
};
#endif // include guard