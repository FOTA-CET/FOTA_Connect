#include <cstdlib>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <csignal>
#include <string>

#include "fota_connect_app.h"
#include "fotaDownload.h"
#include "cloudUrl.h"
#include "jsonKey.h"

std::mutex ecuPercentListMutex;
std::mutex ecuUpdateListMutex;
bool doneAdding = false;

fotaConnectApp::fotaConnectApp()
{
  if (std::getenv("FOTA_STORAGE") == nullptr)
  {
    auto errMsg = "fotaConnectApp: environment variable FOTA_STORAGE is not set";
    throw std::runtime_error(errMsg);
  }
  else
  {
    fotaStorage = std::getenv("FOTA_STORAGE");
    firmwaresMetadataFile = fotaStorage + "/config/firmwareListConfig.json";
    jsonkeyFile = fotaStorage + "/config/firebaseConfig.json";
    tokenFile = fotaStorage + "/config/tokenConfig.json";

    fifoECU = fotaStorage + "/fifoECU";
    fifoFlash = fotaStorage + "/fifoFirmware";
    fifoPercent = fotaStorage + "/fifoPercent";
    fifoStatus = fotaStorage + "/fifoStatus";
    firmwareDir = fotaStorage + "/firmware/";
  }
  jsonKey::handleFirebaseJson(jsonkeyFile);
  jsonKey::handleFirebaseToken(tokenFile);
}

void fotaConnectApp::signalHandler(int signal)
{
  std::cout << "Received signal " << signal << std::endl;
  exit(signal);
}

bool fotaConnectApp::writeFifoPipe(const std::string &fifoPath, std::string &buff)
{
  mkfifo(fifoPath.c_str(), 0666);
  int fd = open(fifoPath.c_str(), O_WRONLY);
  write(fd, buff.c_str(), sizeof(buff));
  close(fd);
  return true;
}

void fotaConnectApp::start()
{
  std::string name;
  fotaDownload object_fotaDownload;

  std::cout << "Starting FOTA CONNECT" << std::endl;

  jsonKey::handleFirebaseJson(jsonkeyFile);
  jsonKey::handleFirebaseToken(tokenFile);

  while (1)
  {
    signal(SIGINT, fotaConnectApp::signalHandler);
    signal(SIGTERM, fotaConnectApp::signalHandler);
    if (!ecuUpdateList.empty())
    {
      {
        std::unique_lock<std::mutex> lock(ecuUpdateListMutex);
        name = ecuUpdateList.front();
        std::cout << "new ECU_UPDATE: " << name << std::endl;
        ecuUpdateList.pop();
        std::cout << "remove: " << name << std::endl;
      }
      object_fotaDownload.setfirmwareMetadata(firmwaresMetadataFile);

      newUpdateInfor = name;

      std::string ecuName = name.substr(0, name.find("_")); // regex
      std::string filePath = firmwareDir + "/" + name;      // concat

      std::cout << "Found new firmware:\t";
      std::cout << name << std::endl;
      std::cout << "Checking firmware\n";
      if (Verification::checkNewestState(name) != Status::OK)
      {
        std::cout << "Firmware already exists.\n";
        if (fotaDownload::updateMCUStatus(ecuName, ECU_StatustoString(ECU_Status::REJECT)))
          std::cout << "Update MCU status OK" << std::endl;
        else
          std::cout << "Update MCU status ERROR" << std::endl;
      }
      else
      {
        std::cout << "Check OK!\n";
        object_fotaDownload.updateMCUStatus(ecuName, ECU_StatustoString(ECU_Status::UPDATE));
        if (strcmp(ecuName.c_str(), "ESP32") != 0)
        {
          filePath += ".hex";
        }
        else
        {
          filePath += ".bin";
        }

        std::string fileName = name + filePath.substr(filePath.length() - 4, 4); // luu y

        if (Verification::checkExistFile(fileName, firmwareDir) == Status::ERROR)
        {
          if (object_fotaDownload.download(stringToECU(ecuName), name, filePath) == Status::OK)
          {
            std::cout << "Download success\n";
          }
          else
          {
            std::cout << "Download fails\n";
          }
        }

        std::cout << "Sending FIFO\n";
        writeFifoPipe(fifoECU, ecuName);
        writeFifoPipe(fifoFlash, fileName);
        std::cout << "ecuName: " << ecuName << std::endl;
        std::cout << "fileName: " << fileName << std::endl;
        std::cout << "Send successful" << std::endl
                  << std::endl;
      }
    }
  }
}

std::string fotaConnectApp::ECUtoString(ECU ecu)
{
  if (ecu == ECU::ESP32)
    return "ESP32";
  else if (ecu == ECU::ATMEGA328P)
    return "ATMEGA328P";
  else
    return "STM32";
}

std::string fotaConnectApp::ECU_StatustoString(ECU_Status status)
{
  if (status == ECU_Status::NONE)
    return "NONE";
  else if (status == ECU_Status::UPDATE)
    return "UPDATE";
  else if (status == ECU_Status::REJECT)
    return "REJECT";
  else return "FAILED";
}

ECU fotaConnectApp::stringToECU(std::string &ecu)
{
  if (!strcmp(ecu.c_str(), "ESP32"))
    return ECU::ESP32;
  else if (!strcmp(ecu.c_str(), "STM32"))
    return ECU::STM32;
  else
    return ECU::ATMEGA328P;
}

bool fotaConnectApp::readFifoPipe(const std::string &fifoPath, std::string &buff)
{
  char buffer[100];
  memset(buffer, 0, sizeof(buffer));
  auto fd = open(fifoPath.c_str(), O_RDONLY);
  if (fd == -1)
  {
    return false;
  }

  auto ret = read(fd, &buffer, sizeof(buffer));
  if (ret <= 0)
  {
    return false;
  }
  buff = buffer;
  close(fd);
  return true;
}

void fotaConnectApp::handlefifoPercent()
{
  std::string percent;
  std::string ecu;
  while (1)
  {
    if (readFifoPipe(fifoPercent, percentBuf)) {
      percent = percentBuf.substr(percentBuf.find("_") + 1);
      ecu = percentBuf.substr(0, percentBuf.find("_"));

      std::unique_lock<std::mutex> lock(ecuPercentListMutex);
      if ((ecuPercentList.find(ecu)) != ecuPercentList.end())
      {
        std::cout << "Add to list " << percent <<  std::endl;
        ecuPercentList[ecu].push_back(percent);

      } else {
        auto result = ecuPercentList.insert(std::make_pair(ecu, std::vector<std::string>()));
        result.first->second.push_back(percent);
      }
    }
  }
}

void fotaConnectApp::updateECUPercentList()
{
  while (1) {
    if (!ecuPercentList.empty()) {

      auto& firstPair = *ecuPercentList.begin();
      auto ecu = firstPair.first;

      auto& percentList = firstPair.second;
      if (!(percentList.empty())) {
        auto it = percentList.begin() ;
        auto percent = *it;

        if (!fotaDownload::updatePercent(ecu, percent)) {
          std::cout << "Update percent fail\n";
        } else {
          if (percent == "100") {
            fotaDownload::updateFirmwareList(newUpdateInfor);
            std::string resetPercent = "0";
            fotaDownload::updateMCUStatus(ecu, ECU_StatustoString(ECU_Status::NONE));
            fotaDownload::updatePercent(ecu, resetPercent);

            std::unique_lock<std::mutex> lock(ecuPercentListMutex);
            ecuPercentList.erase(ecu);
            std::cout << "Successfully to update percent for " << ecu << std::endl;
          } else {
              std::cout << "Uploaded percent " << percent << std::endl;
              std::unique_lock<std::mutex> lock(ecuPercentListMutex);
              
              auto& firstPair = *ecuPercentList.begin();
              auto ecu = firstPair.first;

              auto& percentList = firstPair.second;
              auto it = percentList.begin() ;
              auto percent = *it;
              percentList.erase(it);
              std::cout << "Delected " << percent << std::endl;
          }
        }
      }
    }
  }
}

void fotaConnectApp::handleUpdateTrigger()
{
  std::string name;
  while(1)
  {
    if(fotaDownload::getNameFirmware(name) == Status::OK)
    {
      {
        std::unique_lock<std::mutex> lock(ecuUpdateListMutex);
        ecuUpdateList.push(name);
      }
      fotaDownload::resetUpdateFieldFirebase();
    }
  }
}

void fotaConnectApp::handlefifoStatus()
{
  std::string status;
  std::string ecu;
  std::string resetPercent = "0";
  std::string completedPercent = "100";
  while (1)
  {
    if (readFifoPipe(fifoStatus, statusBuf)) {
      status = statusBuf.substr(statusBuf.find("_") + 1);
      ecu = statusBuf.substr(0, statusBuf.find("_"));

      std::cout << ecu << std::endl;
      std::cout << status << std::endl;

      fotaDownload::updateMCUStatus(ecu, statusBuf.substr(statusBuf.find("_") + 1));
    }
  }
}