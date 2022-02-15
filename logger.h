#ifndef LOGGER_H
#define LOGGER_H

#include "stdout_config.h"
#include "ilogger.h"
#include <string>

using std::string;

//This class is implement by pattern Singlton.
//For getting pointer on class call static method GetLogger().
//Example : Logger* logger = Logger::GetLogger();

class Logger : public ILogger
{
  public:
    static Logger* GetLogger();                                        // Return pointer on Logger.
    void SetSTDOutConfig(const STDOutConfig& config);
    void SetSTDOutConfig(const STDOutMode& mode, const string& path);
    virtual void PrintMessage(const string& message) override;         // Print message in log file.
  private:
    Logger();
    ~Logger();
    static Logger* logger_;
    STDOutConfig* stdout_config_;
    bool first_file_open_flag_;
};

#endif // LOGGER_H
