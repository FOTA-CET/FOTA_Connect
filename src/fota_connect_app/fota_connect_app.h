#ifndef __FOTA_CONNECT_APP_
#define __FOTA_CONNECT_APP_
#include <string>
#include "cloudUrl.h"
#include <thread>
#include <map>
#include <vector>
#include <algorithm>

#include <mutex>

class fotaConnectApp
{
    private:
        static void signalHandler(int signal);
        bool writeFifoPipe(const std::string& fifoPath, std::string& buff);
        bool readFifoPipe(const std::string& fifoPath, std::string& buff);
        std::string fotaStorage;
        std::string fifoECU;
        std::string fifoFlash;
        std::string fifoPercent;
        std::string firmwareDir; 
        std::string firmwaresMetadataFile;
        std::string jsonkeyFile;
        std::string tokenFile;
        std::string percentBuf;
    public:
        fotaConnectApp();
        void start();
        std::string ECUtoString(ECU ecu);
        ECU stringToECU(std::string& ecu);
        std::string ECU_StatustoString(ECU_Status status);
        void handlefifoPercent();
        void updateECUPercentList();
        std::map<std::string, std::vector<std::string>> ecuPercentList;
};
#endif // include guard