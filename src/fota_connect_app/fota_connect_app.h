#ifndef __FOTA_CONNECT_APP_
#define __FOTA_CONNECT_APP_
#include <string>

class fotaConnectApp
{
    private:
        static void signalHandler(int signal);
        bool writeFifoPipe(const std::string& fifoPath, std::string& buff);
        std::string fotaStorage;
        std::string fifoECU;
        std::string fifoFlash;
        std::string listFirmware;
        std::string firmwaresMetadata;
    public:
        fotaConnectApp();
        void configure();
        void start();
};
#endif // include guard