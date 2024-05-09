#include "fotaDownload.h"
#include <iostream>

int main()
{
  std::string name;
  if(fotaDownload::getNameFirmware(name) == fotaDownload::Status::ERROR)
  {
    std::cout << "No new firmware\n";
  }
  else
  {
    std::cout << "Found new firmware:\t";
    std::cout << name << std::endl;
    std::cout << "Checking firmware\n";
    if(fotaDownload::checkNewestState(name) != fotaDownload::Status::OK)
    {
      std::cout << "Firmware is not valid\n";
    }
    else
    {
      std::cout << "Check OK!\n";
      std::string filePath ="/home/thanhtung/Desktop/FOTA_Connect/src/" + name + ".hex";
      if(fotaDownload::download(cloudUrl::ECU::ATMEGA328P, name, filePath) == fotaDownload::Status::OK)
      {
        std::cout << "Update success\n";
      }
      else
      {
        std::cout << "Update fails\n";
      }
    }
  }
  return 0;
}