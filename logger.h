#ifndef LOGGER_H
#define LOGGER_H

#include "stdout_config.h"
#include "ilogger.h"
#include <string>

using std::string;

class Logger : public ILogger
{
  public:
    static Logger* GetLogger(void);
    void SetSTDOutConfig(const STDOutConfig& config);
    void SetSTDOutConfig(const STDOutMode& mode, const string& path);
    void SetSTDOutConfig(const STDOutMode& mode, const char* path);
    virtual void PrintMessage(const string& message) override;
    virtual void PrintMessage(const char* message) override;
  private:
    Logger(void);
    Logger(const STDOutConfig& config);
    Logger(const STDOutMode& mode, const string& path);
    Logger(const STDOutMode& mode, const char* path);
    ~Logger() = default;
    static Logger* logger_;
    STDOutConfig* stdout_config_;
    bool first_file_open_flag_;
};

#endif // LOGGER_H
