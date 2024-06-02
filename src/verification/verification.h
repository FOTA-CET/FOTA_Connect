#ifndef __VERIFICATION_H
#define __VERIFICATION_H
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "fotaDownload.h"
class Verification
{
    public:
        static Status checkNewestState(std::string& fileName);
        static Status checkExistFile(std::string& fileName, std::string& storagePath);
};
#endif // include guard