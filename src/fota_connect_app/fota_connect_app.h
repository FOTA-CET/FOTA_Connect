#ifndef __FOTA_CONNECT_APP_
#define __FOTA_CONNECT_APP_
#include <string>

class fotaConnectApp
{
    private:
        static void signalHandler(int signal);
        bool readFifoPipe(const std::string& fifoPath, std::string& buff);
        std::string fota_conf;
        std::string ecuConfigFile;
        std::string fotaStorage;
        std::string fifoECU;
        std::string fifoFlash;
    public:
        fotaConnectApp();
        void configure();
        void start();
};
#endif // include guard