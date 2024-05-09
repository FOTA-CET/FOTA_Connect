#include "fotaDownload.h"

namespace fotaDownload
{
    Status download(cloudUrl::ECU ecu, std::string& fileName, std::string& path)
    {
        // Tao link tai metadata
        std::string metadataLink = cloudUrl::getCURL(ecu, cloudUrl::typeURL::METADATA, fileName);

        // Lay metadata respone
        std::string metadataRespone;
        if(restAdapter::handleRequest(metadataLink, &metadataRespone) != CURLcode::CURLE_OK) throw -1;

        // Lay token tu respone cua metadataLink
        std::string tokenDownload = jsonKey::getDownloadToken(metadataRespone);

        // Tao link tai firmware
        std::string firmwareLink = cloudUrl::getCURL(ecu, cloudUrl::typeURL::DATA, fileName, tokenDownload);

        // Lay data respone luu vao string
        // std::string dataRespone;
        // if(restAdapter::handleRequest(firmwareLink, &dataRespone) != CURLcode::CURLE_OK) throw -1;
        // std::cout << dataRespone << std::endl;

        // Open the local file for writing response
        FILE *fp = fopen(path.c_str(), "wb");
        if(restAdapter::handleRequest(firmwareLink, &fileName, fp) != CURLcode::CURLE_OK) throw -1;
        fclose(fp);
        return Status::OK;        
    }

    Status getNameFirmware(std::string& nameFirmware)
    {

    }

    cloudUrl::ECU getECUName(std::string& nameFirmware)
    {
        
    }

    Status checkNewestState(cloudUrl::ECU ecu, std::string& fileName)
    {

    }
}