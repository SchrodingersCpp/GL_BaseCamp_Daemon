#include <vector>
#include "data_process.h"

class IProcessLauncher
{
  public:
    virtual void SetProcessData(std::vector<DataProcess>& processes) = 0;  //Set data of prosesses
};
