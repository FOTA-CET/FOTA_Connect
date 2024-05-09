#include "fotaDownload.h"
#include <iostream>

int main()
{
  std::string name;
  fotaDownload object_fotaDownload;
  if(object_fotaDownload.getNameFirmware(name) == Status::ERROR)
  {
    std::cout << "No new firmware\n";
  }
  else
  {
    std::cout << "Found new firmware:\t";
    std::cout << name << std::endl;
    std::cout << "Checking firmware\n";
    if(object_fotaDownload.checkNewestState(name) != Status::OK)
    {
      std::cout << "Firmware is not valid\n";
    }
    else
    {
      std::cout << "Check OK!\n";
      std::string filePath ="/home/thanhtung/Desktop/FOTA_Connect/src/" + name + ".hex";
      if(object_fotaDownload.download(ECU::ATMEGA328P, name, filePath) == Status::OK)
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