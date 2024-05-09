#include "fotaDownload.h"
#include <iostream>

int main()
{
  std::string fileName ="program";
  std::string filePath ="/home/thanhtung/Desktop/FOTA_Connect/src/" + fileName + ".hex";
  fotaDownload::download(cloudUrl::ECU::ATMEGA328P, fileName, filePath);
  return 0;
}