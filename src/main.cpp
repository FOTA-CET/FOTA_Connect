#include "fota_connect_app.h"

int main()
{
  auto a = fotaConnectApp();

    std::thread thread1(&fotaConnectApp::start, &a);
    std::thread thread2(&fotaConnectApp::handlefifoPercent, &a);
    std::thread thread3(&fotaConnectApp::updateECUPercentList, &a);

    thread1.join();
    thread2.join();
    thread3.join();
  return 0;
}