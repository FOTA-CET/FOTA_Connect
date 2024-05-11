#include "fotaDownload.h"

Status fotaDownload::download(ECU ecu, std::string& fileName, std::string& path)
{
    // Tao link tai metadata
    std::string metadataLink = cloudUrl::getCURL(ecu, typeURL::METADATA, fileName);

    // Lay metadata respone
    std::string metadataRespone;
    if(restAdapter::handleRequest(metadataLink, &metadataRespone) != CURLcode::CURLE_OK) throw -1;

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
    if(restAdapter::handleRequest(firmwareLink, &fileName, fp) != CURLcode::CURLE_OK) throw -1;
    fclose(fp);
    return Status::OK;        
}

Status fotaDownload::getNameFirmware(std::string& nameFirmware)
{
    std::string urlFIRMWARE = "https://hex-tung-b8ae7-default-rtdb.firebaseio.com/FIRMWARE.json?BXywcJZCAWi1tiJY0z5hoWz2YwGiVCPqiSOdECW5";

    std::string metadataRespone;
    if(restAdapter::handleRequest(urlFIRMWARE, &metadataRespone) != CURLcode::CURLE_OK) throw -1;

    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    Json::Value root;
    std::string errs;

    bool parsingSuccessful = reader->parse(metadataRespone.c_str(), metadataRespone.c_str() + metadataRespone.size(), &root, &errs);
    delete reader;

    if (!parsingSuccessful) {
        std::cerr << "Failed to parse JSON: " << errs << std::endl;
    }

    std::string MCUname = root["FIRMWARE"].asString();

    if(strcmp(MCUname.c_str(),"NONE") == 0) return Status::ERROR;
    std::string urlMCU = "https://hex-tung-b8ae7-default-rtdb.firebaseio.com/" + MCUname + ".json?BXywcJZCAWi1tiJY0z5hoWz2YwGiVCPqiSOdECW5";

    std::string metadataRespone_MCU;
    if(restAdapter::handleRequest(urlMCU, &metadataRespone_MCU) != CURLcode::CURLE_OK) throw -1;

    Json::CharReaderBuilder builder_2;
    Json::CharReader* reader_2 = builder.newCharReader();
    Json::Value root_2;
    std::string errs_2;

    bool parsingSuccessful_2 = reader_2->parse(metadataRespone_MCU.c_str(), metadataRespone_MCU.c_str() + metadataRespone_MCU.size(), &root_2, &errs_2);
    delete reader_2;

    if (!parsingSuccessful_2) {
        std::cerr << "Failed to parse JSON: " << errs_2 << std::endl;
    }

    std::string version = root_2["version"].asString();

    nameFirmware = MCUname;
    nameFirmware += "_";
    nameFirmware += version;
    return Status::OK;
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

Status fotaDownload::checkNewestState(std::string& fileName)
{
    std::ifstream file(firmwareMetadataDir);
    Json::Value root;
    file >> root;
    if (root.isMember(getECUName(fileName))) {
    std::string result = root[getECUName(fileName)].asString();
    if(strcmp(getFirmwareVersion(fileName).c_str(), result.c_str()) != 0) return Status::OK;
    else return Status::ERROR;
    }
    return Status::ERROR;
}

bool fotaDownload::updateFirmwareList(std::string& newName)
{
    std::ifstream file(firmwareMetadataDir);
    Json::Value root;
    file >> root;

    // Chỉnh sửa dữ liệu
    root[getECUName(newName)] = getFirmwareVersion(newName);

    // Lưu lại các thay đổi vào tệp JSON
    std::ofstream outFile(firmwareMetadataDir);
    outFile << root;
    outFile.close();
    return true;
}

void fotaDownload::setfirmwareMetadata(std::string& firmwareMetadata)
{
    this->firmwareMetadataDir = firmwareMetadata;
}

ECU fotaDownload::stringToECU(std::string& ecuName)
{
    if(!strcmp(ecuName.c_str(),"ESP32")) return ECU::ESP32;
    else if(!strcmp(ecuName.c_str(),"ATMEGA328P")) return ECU::ATMEGA328P;
    else return ECU::STM32;
}
