#include "restAdapter.h"
#include "jsonKey.h"

size_t restAdapter::capture_data(void *ptr, size_t size, size_t nmemb, void *userp) {
size_t chunk_size = size * nmemb;
std::string *data = static_cast<std::string*>(userp);
data->append(static_cast<char*>(ptr), chunk_size);
return chunk_size;
}

size_t restAdapter::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

CURLcode restAdapter::handleRequest(std::string& url, std::string* responeString, FILE *stream)
{
    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        std::string privateKey = jsonKey::getPrivateKey(); // Path to your service account JSON file
        curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, privateKey.c_str());

        if(stream == nullptr)
        {
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, capture_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, responeString);
        }
        else
        {
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, stream);
        }

        CURLcode res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        return CURLcode::CURLE_OK;
    } 
    else 
    {
        return CURLcode::CURLE_FAILED_INIT;
    }
}