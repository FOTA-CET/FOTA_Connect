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
  if (std::getenv("FOTA_STORAGE") == nullptr) {
    auto errMsg = "fotaConnectApp: environment variable FOTA_STORAGE is not set";
    throw std::runtime_error(errMsg);
  } else {
    fotaStorage = std::getenv("FOTA_STORAGE");
    fifoECU = fotaStorage + "/fifoECU";
    fifoFlash = fotaStorage + "/fifoFlash";
    listFirmware = fotaStorage + "/Firmware/";
  }

  if (std::getenv("FOTA_FIMWARE_LIST") == nullptr) {
    auto errMsg = "fotaConnectApp: environment variable FOTA_FIMWARE_LIST is not set";
    throw std::runtime_error(errMsg);
  }
  else
  {
    firmwaresMetadata = std::getenv("FOTA_FIMWARE_LIST");
  }

  if (std::getenv("FIREBASE_JSON_KEY") == nullptr) {
    auto errMsg = "fotaConnectApp: environment variable FIREBASE_JSON_KEY is not set";
    throw std::runtime_error(errMsg);
  }
  else
  {
    jsonkey = std::getenv("FIREBASE_JSON_KEY");
  }

  std::cout << "fotaStorage: " << fotaStorage << std::endl;
  std::cout << "fifoECU: " << fifoECU << std::endl;
  std::cout << "fifoFlash: " << fifoFlash << std::endl;
  std::cout << "Firmware: " << listFirmware << std::endl;
  std::cout << "firmwaresMetadata: " << firmwaresMetadata << std::endl;
}

void fotaConnectApp::signalHandler(int signal) {
  std::cout << "Received signal " << signal << std::endl;
  exit(signal);
}

bool fotaConnectApp::writeFifoPipe(const std::string& fifoPath, std::string& buff) {
  // Mở hoặc tạo FIFO
    mkfifo(fifoPath.c_str(), 0666);

    // Mở FIFO để ghi
    int fd = open(fifoPath.c_str(), O_WRONLY);

    write(fd, buff.c_str(), buff.length());

    // Đóng FIFO
    close(fd);

    return true;
}

void fotaConnectApp::start()
{
  std::string name;
  fotaDownload object_fotaDownload;

  jsonKey::handleFirebaseJson(jsonkey);
  jsonKey::handleFirebaseToken(firmwaresMetadata);

  while(object_fotaDownload.getNameFirmware(name) == Status::ERROR);
  object_fotaDownload.setfirmwareMetadata(firmwaresMetadata);
  std::cout << "Found new firmware:\t";
  std::cout << name << std::endl;
  std::cout << "Checking firmware\n";
  if(object_fotaDownload.checkNewestState(name) != Status::OK)
  {
    std::cout << "Firmware already exists.\n";
  }
  else
  {
    std::cout << "Check OK!\n";
    std::string ecuName = name.substr(0,name.find("_"));
    std::string filePath = listFirmware + "/" + name;
    std::string fileName = name;
    if(object_fotaDownload.stringToECU(ecuName) != ECU::ESP32) 
    {
      filePath += ".hex";
      fileName += ".hex";
    }
    else 
    {
      filePath += ".bin";
      fileName += ".bin";
    }
    if(object_fotaDownload.download(object_fotaDownload.stringToECU(ecuName), name, filePath) == Status::OK)
    {
      std::cout << "Download success\n";
      std::cout << "Sending FIFO\n";
      writeFifoPipe(fifoECU, ecuName);
      writeFifoPipe(fifoFlash,fileName);
      std::cout << "Send sucessful\n" << std::endl;
      object_fotaDownload.updateFirmwareList(name);
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