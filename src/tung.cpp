#include <iostream>
#include <curl/curl.h>

int main() {
    // Khởi tạo Curl
    CURL *curl;
    CURLcode res;

    // URL của Firebase Realtime Database
    const char *url = "https://hex-tung-b8ae7-default-rtdb.firebaseio.com/ATMEGA328P.json?BXywcJZCAWi1tiJY0z5hoWz2YwGiVCPqiSOdECW5";

    // Khởi tạo Curl
    curl = curl_easy_init();
    if (curl) {
        // Cài đặt URL và header
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Thực hiện yêu cầu GET
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Giải phóng Curl và danh sách header
        curl_easy_cleanup(curl);
    }

    return 0;
}
