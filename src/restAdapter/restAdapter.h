#ifndef __REST_ADAPTER__
#define __REST_ADAPTER__
#include <iostream>
#include <fstream>
#include <curl/curl.h>

namespace restAdapter
{
    static size_t capture_data(void *ptr, size_t size, size_t nmemb, void *userp);
    size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
    CURLcode handleRequest(std::string& url, std::string* responeString = nullptr, FILE *stream = nullptr);
}
#endif