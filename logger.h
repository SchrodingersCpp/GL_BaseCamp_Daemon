#include "stdout_config.h"
#include "ilogger.h"
#include <string>

using std::string;

class Logger : public ILogger
{
  public:
    static Logger* GetLogger(void);
    void SetSTDOutConfig(STDOutConfig& config);
    virtual void PrintMessage(string& message) override;
  private:
    Logger();
    ~Logger() = default;
    static Logger* logger_;
    STDOutConfig* stdout_config_;
    bool first_file_open_flag_;
};
