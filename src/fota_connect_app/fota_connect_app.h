#ifndef __FOTA_CONNECT_APP_
#define __FOTA_CONNECT_APP_
#include <string>
#include "cloudUrl.h"
#include "verification.h"
#include <thread>
#include <map>
#include <vector>
#include <queue>
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
        std::string fifoStatus;
        std::string fifoReset;
        std::string firmwareDir; 
        std::string firmwaresMetadataFile;
        std::string jsonkeyFile;
        std::string tokenFile;
        std::string percentBuf;
        std::string statusBuf;
        std::string newUpdateInfor;
        std::string ECUtoString(ECU ecu);
        ECU stringToECU(std::string& ecu);
        std::string ECU_StatustoString(ECU_Status status);
        std::map<std::string, std::vector<std::string>> ecuPercentList;
        std::queue <std::string> ecuUpdateList;
    public:
        fotaConnectApp();
        void start();
        void handlefifoPercent();
        void handlefifoStatus();
        void updateECUPercentList();
        void handleUpdateTrigger();
        void handleFactoryResetTrigger();
};
#endif // include guard