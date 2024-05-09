#include "jsonKey.h"

std::string jsonKey::getDownloadToken(const std::string& jsonString) {
    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    Json::Value root;
    std::string errs;

    bool parsingSuccessful = reader->parse(jsonString.c_str(), jsonString.c_str() + jsonString.size(), &root, &errs);
    delete reader;

    if (!parsingSuccessful) {
        std::cerr << "Failed to parse JSON: " << errs << std::endl;
        return ""; // Trả về chuỗi rỗng nếu có lỗi xảy ra
    }

    if (root.isMember("downloadTokens")) {
        return root["downloadTokens"].asString();
    } else {
        std::cerr << "Failed to find downloadTokens key in JSON." << std::endl;
        return ""; // Trả về chuỗi rỗng nếu không tìm thấy khóa "downloadTokens"
    }
}

// Function to read the service account JSON file and extract the private key
std::string jsonKey::getPrivateKey(const std::string& serviceAccountFile) {
  std::ifstream file(serviceAccountFile);
  if (!file.is_open()) {
    std::cerr << "Failed to open service account file." << std::endl;
    return "";
  }

  Json::Value root;
  file >> root;
  return root["private_key"].asString();
}