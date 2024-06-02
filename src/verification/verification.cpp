#include "verification.h"

Status Verification::checkNewestState(std::string& fileName)
{
    std::ifstream file(fotaDownload::firmwareMetadataDir);
    Json::Value root;
    file >> root;
    if (root.isMember(fotaDownload::getECUName(fileName))) {
    std::string result = root[fotaDownload::getECUName(fileName)].asString();
    if(strcmp(fotaDownload::getFirmwareVersion(fileName).c_str(), result.c_str()) != 0) return Status::OK;
    else return Status::ERROR;
    }
    return Status::ERROR;
}

Status Verification::checkExistFile(std::string& fileName, std::string& storagePath)
{
    std::string filePath = storagePath + fileName;
    if (access(filePath.c_str(), F_OK) != -1) {
        return Status::OK;
    } else {
        return Status::ERROR;
    }
}