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
void jsonKey::handleFirebaseJson(const std::string& serviceAccountFile) {
  std::ifstream file(serviceAccountFile);
  if (!file.is_open()) {
    std::cerr << "Failed to handleFirebaseJson" << std::endl;
    throw -1;
  }

  Json::Value root;
  file >> root;
  ProjectID = root["project_id"].asString();
  PrivateKey = root["private_key"].asString();
}

void jsonKey::handleFirebaseToken(const std::string& serviceAccountFile)
{
  std::ifstream file(serviceAccountFile);
  if (!file.is_open()) {
    std::cerr << "Failed to handleFirebaseToken" << std::endl;
    throw -1;
  }

  Json::Value root;
  file >> root;
 Token = root["token"].asString();
}

std::string jsonKey::getPrivateKey()
{
  return PrivateKey;
}
std::string jsonKey::getProjectID()
{
  return ProjectID;
}

std::string jsonKey::getToken()
{
  return Token;
}

std::string jsonKey::PrivateKey = "";
std::string jsonKey::ProjectID = "";
std::string jsonKey::Token = "";