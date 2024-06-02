#include "fota_connect_app.h"

int main()
{
  auto a = fotaConnectApp();

    std::thread thread1(&fotaConnectApp::start, &a);
    std::thread thread2(&fotaConnectApp::handlefifoPercent, &a);
    std::thread thread3(&fotaConnectApp::updateECUPercentList, &a);
    std::thread thread4(&fotaConnectApp::handleUpdateTrigger, &a);
    std::thread thread5(&fotaConnectApp::handlefifoStatus, &a);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
  return 0;
}