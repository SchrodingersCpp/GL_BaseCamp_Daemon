#ifndef LOGGER_H
#define LOGGER_H

#include "stdout_config.h"
#include "ilogger.h"
#include <string>
#include <fstream>

//This class is implement by pattern Singlton.
//For getting pointer on class call static method GetLogger().
//Example : Logger* logger = Logger::GetLogger();

class Logger : public ILogger
{
  public:
    static Logger* GetLogger();                                             // Return pointer on Logger.
    static void DeleteLogger();
    void SetSTDOutConfig(const STDOutConfig& config);
    void SetSTDOutConfig(const STDOutMode& mode, const std::string& path);
    virtual void PrintMessage(const std::string& message) override;         // Print message in log file.
  private:
    Logger();
    ~Logger();
    static Logger* logger_;
    STDOutConfig stdout_config_;
    std::ofstream logger_out_;
};

#endif // LOGGER_H
