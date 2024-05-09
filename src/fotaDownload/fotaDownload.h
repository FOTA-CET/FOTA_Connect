#ifndef __FOTA_DOWNLOAD_
#define __FOTA_DOWNLOAD_

#include <iostream>

#include "cloudUrl.h"
#include "restAdapter.h"
#include "jsonKey.h"
#include "cloudUrl.h"
#include <json/json.h> 
#include <string.h>

namespace fotaDownload
{
    enum class Status{OK, ERROR};
    Status download(cloudUrl::ECU ecu, std::string& fileName, std::string& path);
    Status getNameFirmware(std::string& nameFirmware);
    Status checkNewestState(std::string& fileName);
    std::string getECUName(std::string& nameFirmware);
    std::string getFirmwareVersion(std::string& nameFirmware);

}
#endif // include guard