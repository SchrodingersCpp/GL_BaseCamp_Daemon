#include <string>

using std::string;

//This class is implement pattern Singlton.
//For getting pointer on class call static method GetLogger()
//Example : Logger* logger = Logger::GetLogger();

class ILogger
{
  public:
    virtual void PrintMessage(const string& message) = 0;  //Print message in log
    virtual void PrintMessage(const char* message) = 0;
};
