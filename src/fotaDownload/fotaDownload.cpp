#include "fotaDownload.h"

Status fotaDownload::download(ECU ecu, std::string& fileName, std::string& path)
{
    // Tao link tai metadata
    std::string metadataLink = cloudUrl::getCURL(ecu, typeURL::METADATA, fileName);

    // Lay metadata respone
    std::string metadataRespone;
    if(restAdapter::handleRequest(metadataLink, &metadataRespone) != CURLcode::CURLE_OK) Status::ERROR;

    // Lay token tu respone cua metadataLink
    std::string tokenDownload = jsonKey::getDownloadToken(metadataRespone);

    // Tao link tai firmware
    std::string firmwareLink = cloudUrl::getCURL(ecu, typeURL::DATA, fileName, tokenDownload);

    // Lay data respone luu vao string
    // std::string dataRespone;
    // if(restAdapter::handleRequest(firmwareLink, &dataRespone) != CURLcode::CURLE_OK) throw -1;
    // std::cout << dataRespone << std::endl;

    // Open the local file for writing response
    FILE *fp = fopen(path.c_str(), "wb");
    if(restAdapter::handleRequest(firmwareLink, &fileName, fp) != CURLcode::CURLE_OK) Status::ERROR;
    fclose(fp);
    return Status::OK;        
}

Status fotaDownload::getNameFirmware(std::string& nameFirmware)
{
    std::string urlFIRMWARE = "https://";
    urlFIRMWARE += jsonKey::getProjectID();
    urlFIRMWARE += "-default-rtdb.firebaseio.com/.json?auth=";
    urlFIRMWARE += jsonKey::getToken();

    std::string metadataRespone;
    if(restAdapter::handleRequest(urlFIRMWARE, &metadataRespone) != CURLcode::CURLE_OK) throw -1;

    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    Json::Value root;
    std::string errs;

    bool parsingSuccessful = reader->parse(metadataRespone.c_str(), metadataRespone.c_str() + metadataRespone.size(), &root, &errs);
    delete reader;

    if (!parsingSuccessful) {
        // std::cerr << "Failed to parse JSON: " << errs << std::endl;
        return Status::ERROR;
    }

    std::string MCUname = root["ECU_UPDATE"].asString();

    if(strcmp(MCUname.c_str(),"NONE") == 0) return Status::ERROR;
    std::string urlMCU = "https://";
    urlMCU += jsonKey::getProjectID();;
    urlMCU += "-default-rtdb.firebaseio.com/ECU/" + MCUname;
    urlMCU += ".json?auth=";
    urlMCU += jsonKey::getToken();

    std::string metadataRespone_MCU;
    if(restAdapter::handleRequest(urlMCU, &metadataRespone_MCU) != CURLcode::CURLE_OK) throw -1;

    Json::CharReaderBuilder builder_2;
    Json::CharReader* reader_2 = builder.newCharReader();
    Json::Value root_2;
    std::string errs_2;

    bool parsingSuccessful_2 = reader_2->parse(metadataRespone_MCU.c_str(), metadataRespone_MCU.c_str() + metadataRespone_MCU.size(), &root_2, &errs_2);
    delete reader_2;

    if (!parsingSuccessful_2) {
        // std::cerr << "Failed to parse JSON: " << errs_2 << std::endl;
        return Status::ERROR;
    }

    std::string version = root_2["version"].asString();

    nameFirmware = MCUname;
    nameFirmware += "_";
    nameFirmware += version;
    return Status::OK;
}

Status fotaDownload::getFactoryResetName(std::string& factoryResetName)
{
    std::string urlFIRMWARE = "https://";
    urlFIRMWARE += jsonKey::getProjectID();
    urlFIRMWARE += "-default-rtdb.firebaseio.com/.json?auth=";
    urlFIRMWARE += jsonKey::getToken();

    std::string metadataRespone;
    if(restAdapter::handleRequest(urlFIRMWARE, &metadataRespone) != CURLcode::CURLE_OK) throw -1;

    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    Json::Value root;
    std::string errs;

    bool parsingSuccessful = reader->parse(metadataRespone.c_str(), metadataRespone.c_str() + metadataRespone.size(), &root, &errs);
    delete reader;

    if (!parsingSuccessful) {
        // std::cerr << "Failed to parse JSON: " << errs << std::endl;
        return Status::ERROR;
    }

    factoryResetName = root["ECU_RESET"].asString();

    if(strcmp(factoryResetName.c_str(),"NONE") == 0) return Status::ERROR;
    else return Status::OK;
}

std::string fotaDownload::getECUName(std::string& nameFirmware)
{
    std::string ECUname = nameFirmware.substr(0,nameFirmware.find('_'));
    return ECUname;
}

std::string fotaDownload::getFirmwareVersion(std::string& nameFirmware)
{
    std::string firmwareVersion = nameFirmware.substr(nameFirmware.find('_') + 1);
    return firmwareVersion;
}

bool fotaDownload::updateFirmwareList(std::string& newName)
{
    std::ifstream file(firmwareMetadataDir);
    Json::Value root;
    file >> root;

    // Chỉnh sửa dữ liệu
    root[fotaDownload::getECUName(newName)] = fotaDownload::getFirmwareVersion(newName);

    // Lưu lại các thay đổi vào tệp JSON
    std::ofstream outFile(firmwareMetadataDir);
    outFile << root;
    outFile.close();
    return true;
}

void fotaDownload::setfirmwareMetadata(std::string& firmwareMetadata)
{
    firmwareMetadataDir = firmwareMetadata;
}

bool fotaDownload::resetUpdateFieldFirebase()
{
    if(restAdapter::writeFirebase(".json", "ECU_UPDATE", "NONE")) return true;
    else return false;
}

bool fotaDownload::resetFactoryResetFieldFirebase()
{
    if(restAdapter::writeFirebase(".json", "ECU_RESET", "NONE")) return true;
    else return false;
}

bool fotaDownload::updateMCUStatus(std::string& ecu, std::string&& status)
{
    if(restAdapter::writeFirebase("ECU/" + ecu + ".json", "status", status)) return true;
    else return false;
}

bool fotaDownload::updatePercent(std::string& ecu, std::string& percent)
{
    if(restAdapter::writeFirebase("ECU/" + ecu + ".json", "percent", percent)) return true;
    else return false;
}

std::string fotaDownload::firmwareMetadataDir = "";