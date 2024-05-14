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
  if (std::getenv("FOTA_CONFIG_DIR") == nullptr) {
    auto errMsg = "fotaConnectApp: environment variable FOTA_CONFIG_DIR is not set";
    throw std::runtime_error(errMsg);
  } else {
    fotaConfDir = std::getenv("FOTA_CONFIG_DIR");
    firmwaresMetadataFile = fotaConfDir + "/firmwareList.json";
    jsonkeyFile = fotaConfDir + "/firebaseConfig.json";
    tokenFile = fotaConfDir + "/tokenConfig.json";
  }

  if (std::getenv("FOTA_STORAGE") == nullptr) {
    auto errMsg = "fotaConnectApp: environment variable FOTA_STORAGE is not set";
    throw std::runtime_error(errMsg);
  }
  else
  {
    fotaStorage = std::getenv("FOTA_STORAGE");
    fifoECU = fotaStorage + "/fifoECU";
    fifoFlash = fotaStorage + "/fifoFlash";
    firmwareDir = fotaStorage; // + "/Firmware/";
  }
}

void fotaConnectApp::signalHandler(int signal) {
  std::cout << "Received signal " << signal << std::endl;
  exit(signal);
}

bool fotaConnectApp::writeFifoPipe(const std::string& fifoPath, std::string& buff) {
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

  while(object_fotaDownload.getNameFirmware(name) == Status::ERROR);
  object_fotaDownload.setfirmwareMetadata(firmwaresMetadataFile);
  std::cout << "Found new firmware:\t";
  std::cout << name << std::endl;
  std::cout << "Checking firmware\n";
  if(object_fotaDownload.checkNewestState(name) != Status::OK)
  {
    std::cout << "Firmware already exists.\n";
    restAdapter::resetStatusFirebase();
  }
  else
  {
    std::cout << "Check OK!\n";
    std::string ecuName = name.substr(0,name.find("_"));
    std::string filePath = firmwareDir + "/" + name;
    if(object_fotaDownload.stringToECU(ecuName) != ECU::ESP32) 
    {
      filePath += ".hex";
    }
    else 
    {
      filePath += ".bin";
    }

    std::string fileName = name + filePath.substr(filePath.length() - 4, 4);
    if(object_fotaDownload.download(object_fotaDownload.stringToECU(ecuName), name, filePath) == Status::OK)
    {
      std::cout << "Download success\n";
      std::cout << "Sending FIFO\n";
      writeFifoPipe(fifoECU, ecuName);
      writeFifoPipe(fifoFlash,fileName);
      std::cout << "Send sucessful" << std::endl;
      object_fotaDownload.updateFirmwareList(name);
      restAdapter::resetStatusFirebase();
    }
    else
    {
      std::cout << "Download fails\n";
    }
  }

  signal(SIGINT, fotaConnectApp::signalHandler);
  signal(SIGTERM, fotaConnectApp::signalHandler);
  fotaConnectApp::start();
}