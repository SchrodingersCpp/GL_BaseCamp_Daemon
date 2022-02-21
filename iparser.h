#ifndef IPARSER_H
#define IPARSER_H

#include <string>
#include <vector>
#include "data_process.h"

class IParser
{
  public:
    virtual void SetFilePath(const std::string& path) = 0; // set path of process config file
    virtual std::vector<DataProcess> GetProcessData() = 0; // return collection of struct DataProcess
};

#endif // IPARSER_H
