#include <string>

using std::string;

class ILogger
{
  public:
    virtual ILogger* GetLogger(void) = 0;            //Return pointer on object ILogger. This class is implement pattern Singlton. 
    virtual void PrintMessage(string& message) = 0;  //Print message in log
};
