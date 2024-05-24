#ifndef __FOTA_DOWNLOAD_
#define __FOTA_DOWNLOAD_

#include <iostream>

#include "cloudUrl.h"
#include "restAdapter.h"
#include "jsonKey.h"
#include "cloudUrl.h"
#include <json/json.h> 
#include <string.h>
#include <unistd.h>

enum class Status{OK, ERROR};
class fotaDownload
{
    private:
        std::string getECUName(std::string& nameFirmware);
        std::string getFirmwareVersion(std::string& nameFirmware);
        std::string firmwareMetadataDir;
    public:
        static Status getNameFirmware(std::string& nameFirmware);
        Status download(ECU ecu, std::string& fileName, std::string& path);
        Status checkNewestState(std::string& fileName);
        Status checkExistFile(std::string& fileName, std::string& storagePath);
        bool updateFirmwareList(std::string& newName);
        void setfirmwareMetadata(std::string& firmwareMetadata);
        bool resetUpdateFieldFirebase();
        static bool updateMCUStatus(std::string& ecu, std::string&& status);
        static bool updatePercent(std::string& ecu, std::string& percent);
};
#endif // include guard