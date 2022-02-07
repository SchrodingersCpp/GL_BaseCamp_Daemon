#include <string>
#include <vector>
#include "data_process.h"

using std::string;
using std::vector;

class IParser
{
  public:
    virtual void SetFilePath(string& path) = 0;              //Set path of process config file
    virtual vector<DataProcess>* GetProcessData(void) = 0;   //Return pointer on collection of struct DataProcess, if collection of struct DataProcess is empty return nullptr
};
