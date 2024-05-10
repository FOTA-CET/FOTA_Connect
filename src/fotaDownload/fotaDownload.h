#ifndef __FOTA_DOWNLOAD_
#define __FOTA_DOWNLOAD_

#include <iostream>

#include "cloudUrl.h"
#include "restAdapter.h"
#include "jsonKey.h"
#include "cloudUrl.h"
#include <json/json.h> 
#include <string.h>

enum class Status{OK, ERROR};
class fotaDownload
{
    private:
        std::string getECUName(std::string& nameFirmware);
        std::string getFirmwareVersion(std::string& nameFirmware);
    public:
        Status getNameFirmware(std::string& nameFirmware);
        Status download(ECU ecu, std::string& fileName, std::string& path);
        Status checkNewestState(std::string& fileName);
        bool updateFirmwareList(std::string& newName);

};
#endif // include guard