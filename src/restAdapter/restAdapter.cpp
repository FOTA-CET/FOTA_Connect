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

size_t restAdapter::skipResponse(void *contents, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;
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

bool restAdapter::resetStatusFirebase()
{
    std::string urlFIRMWARE = "https://";
    urlFIRMWARE += jsonKey::getProjectID();
    urlFIRMWARE += "-default-rtdb.firebaseio.com/FIRMWARE.json?";
    urlFIRMWARE += jsonKey::getToken();

    CURL *curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, urlFIRMWARE.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        const char *postData = "{\"FIRMWARE\": \"NONE\"}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, skipResponse);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Failed to perform request: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Reset state OK\n" << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    else 
    {
        std::cerr << "Failed to initialize cURL." << std::endl;
    }
    return true;
}