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

fotaConnectApp::fotaConnectApp()
{
  if (std::getenv("FOTA_STORAGE") == nullptr) {
    auto errMsg = "fotaMasterApp: environment variable FOTA_STORAGE is not set";
    throw std::runtime_error(errMsg);
  } else {
    fotaStorage = std::getenv("FOTA_STORAGE");
    fifoECU = fotaStorage + "/fifoECU";
    fifoFlash = fotaStorage + "/fifoFlash";
  }
  std::cout << "fotaStorage: " << fotaStorage << std::endl;
  std::cout << "fifoECU: " << fifoECU << std::endl;
  std::cout << "fifoFlash: " << fifoFlash << std::endl;
}

void fotaConnectApp::signalHandler(int signal) {
  std::cout << "Received signal " << signal << std::endl;
  exit(signal);
}

bool fotaConnectApp::readFifoPipe(const std::string& fifoPath, std::string& buff) {
  char buffer[100];
  auto fd = open(fifoPath.c_str(), O_RDONLY);
  if (fd == -1) {
    return false;
  }

  auto ret = read(fd, &buffer, sizeof(buffer));
  if (ret <= 0) {
    return false;
  }
  buff = buffer;
  close(fd);
  return true;
}

void fotaConnectApp::start()
{
  std::string name;
  fotaDownload object_fotaDownload;
  while(object_fotaDownload.getNameFirmware(name) == Status::ERROR);
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
    std::string filePath = fifoFlash + "/" + name + ".hex";
    object_fotaDownload.updateFirmwareList(name);
    if(object_fotaDownload.download(ECU::ATMEGA328P, name, filePath) == Status::OK)
    {
      std::cout << "Update success\n";
    }
    else
    {
      std::cout << "Update fails\n";
    }
  }

  signal(SIGINT, fotaConnectApp::signalHandler);
  signal(SIGTERM, fotaConnectApp::signalHandler);
  fotaConnectApp::start();
}