#include <cstdlib>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <csignal>

#include "fota_connect_app.h"
#include "fotaDownload.h"
#include "cloudUrl.h"
#include "jsonKey.h"

fotaConnectApp::fotaConnectApp()
{
  if (std::getenv("FOTA_CONFIG_DIR") == nullptr)
  {
    auto errMsg = "fotaConnectApp: environment variable FOTA_CONFIG_DIR is not set";
    throw std::runtime_error(errMsg);
  }
  else
  {
    fotaConfDir = std::getenv("FOTA_CONFIG_DIR");
    firmwaresMetadataFile = fotaConfDir + "/firmwareList.json";
    jsonkeyFile = fotaConfDir + "/firebaseConfig.json";
    tokenFile = fotaConfDir + "/tokenConfig.json";
  }

  if (std::getenv("FOTA_STORAGE") == nullptr)
  {
    auto errMsg = "fotaConnectApp: environment variable FOTA_STORAGE is not set";
    throw std::runtime_error(errMsg);
  }
  else
  {
    fotaStorage = std::getenv("FOTA_STORAGE");
    fifoECU = fotaStorage + "/fifoECU";
    fifoFlash = fotaStorage + "/fifoFlash";
    fifoPercent = fotaStorage + "/fifoProgress";
    firmwareDir = fotaStorage; // + "/Firmware/";
  }
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
  write(fd, buff.c_str(), buff.length());
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
    if (object_fotaDownload.getNameFirmware(name) == Status::OK)
    {
      object_fotaDownload.setfirmwareMetadata(firmwaresMetadataFile);

      std::string ecuName = name.substr(0, name.find("_")); // regex
      std::string filePath = firmwareDir + "/" + name;      // concat

      std::cout << "Found new firmware:\t";
      std::cout << name << std::endl;
      std::cout << "Checking firmware\n";
      if (object_fotaDownload.checkNewestState(name) != Status::OK)
      {
        std::cout << "Firmware already exists.\n";
        if (object_fotaDownload.resetUpdateFieldFirebase())
          std::cout << "Reset state OK" << std::endl;
        else
          std::cout << "Reset state ERROR\n"
                    << std::endl;
        if (object_fotaDownload.updateMCUStatus(ecuName, ECU_StatustoString(ECU_Status::REJECT)))
          std::cout << "Update MCU status OK" << std::endl;
        else
          std::cout << "Update MCU status ERROR" << std::endl;
      }
      else
      {
        std::cout << "Check OK!\n";
        if (strcmp(ecuName.c_str(), "ESP32") != 0)
        {
          filePath += ".hex";
        }
        else
        {
          filePath += ".bin";
        }

        std::string fileName = name + filePath.substr(filePath.length() - 4, 4); // luu y
        if (object_fotaDownload.download(stringToECU(ecuName), name, filePath) == Status::OK)
        {
          object_fotaDownload.updateMCUStatus(ecuName, ECU_StatustoString(ECU_Status::DOWNLOAD));
          std::cout << "Download success\n";

          object_fotaDownload.resetUpdateFieldFirebase();
          object_fotaDownload.updateFirmwareList(name);

          std::cout << "Sending FIFO\n";
          writeFifoPipe(fifoECU, ecuName);
          writeFifoPipe(fifoFlash, fileName);
          std::cout << "Send successful" << std::endl;
        }
        else
        {
          std::cout << "Download fails\n";
        }
      }
    }
    signal(SIGINT, fotaConnectApp::signalHandler);
    signal(SIGTERM, fotaConnectApp::signalHandler);
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
  else
    return "REJECT";
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

void fotaConnectApp::handleProgress()
{
  while (1)
  {
    if (readFifoPipe(fifoPercent, percentBuf))
    {
      std::string percent = percentBuf.substr(percentBuf.find("_") + 1);
      std::string ecu = percentBuf.substr(0, percentBuf.find("_"));
      if (restAdapter::writeFirebase(ecu + ".json", "percent", percent))
      {
        std::cout << "update percent successful\n";
      }

      else
      {
        std::cout << "Update percent fail\n";
      }
    }
  }
}