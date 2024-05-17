#include "fota_connect_app.h"

int main()
{
  auto a = fotaConnectApp();

    std::thread thread1(&fotaConnectApp::start, &a);
    std::thread thread2(&fotaConnectApp::handleProgress, &a);

    thread1.join();
    thread2.join();
  return 0;
}