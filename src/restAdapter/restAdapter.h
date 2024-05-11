#ifndef __REST_ADAPTER__
#define __REST_ADAPTER__
#include <iostream>
#include <fstream>
#include <curl/curl.h>

class restAdapter
{
    private:
        static size_t capture_data(void *ptr, size_t size, size_t nmemb, void *userp);
        static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
        static size_t skipResponse(void *contents, size_t size, size_t nmemb, void *userp);
    public:
        static CURLcode handleRequest(std::string& url, std::string* responeString = nullptr, FILE *stream = nullptr);
        static bool resetStatusFirebase();
};
#endif