#ifndef __FOTA_DOWNLOAD_
#define __FOTA_DOWNLOAD_

#include <iostream>

#include "cloudUrl.h"
#include "restAdapter.h"
#include "jsonKey.h"
#include "cloudUrl.h"

namespace fotaDownload
{
    enum class Status{OK, ERROR};
    Status download(cloudUrl::ECU ecu, std::string& fileName, std::string& path);
    Status getNameFirmware(std::string& nameFirmware);
    Status checkNewestState(cloudUrl::ECU ecu, std::string& fileName);
    cloudUrl::ECU getECUName(std::string& nameFirmware);
}
#endif // include guard