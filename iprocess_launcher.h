#include <string>
#include <vector>
#include "data_process.h"

using std::string;
using std::vector;

class IProcessLauncher
{
  public:
    virtual void SetProcessData(const vector<DataProcess>& processes) = 0;  //Set data of prosesses
};
