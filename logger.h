#include "ilogger.h"
#include <string>

using std::string;

class Logger : public ILogger
{
  public:
    static Logger* GetLogger(void);
    virtual void PrintMessage(string& message) override;
  private:
    Logger() = default;
    ~Logger() = default;
    static Logger* logger_;
};
