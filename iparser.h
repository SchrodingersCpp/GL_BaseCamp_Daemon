#include <string>
#include <vector>
#include "data_process.h"

class IParser
{
  public:
    virtual void SetFilePath(const std::string& path) = 0;   //Set path of process config file
    virtual std::vector<DataProcess>* GetProcessData() = 0;  //Return pointer on collection of struct DataProcess, if collection of struct DataProcess is empty return nullptr
};
