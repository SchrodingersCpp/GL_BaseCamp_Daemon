#include "logger.h"
#include <ctime>

Logger* Logger::logger_ = nullptr;

Logger::Logger()
{
  const std::string kLogDefaultPathFile = "./log.txt";
  SetSTDOutConfig(STDOutMode::kSTDOutModeTruncate, kLogDefaultPathFile);
}

Logger::~Logger()
{
  if(logger_out_.is_open())
  {
    logger_out_.close();
  }
}

Logger* Logger::GetLogger()
{
  if(logger_ == nullptr)
  {
    logger_ = new Logger;
  }
  return logger_;
}

void Logger::DeleteLogger()
{
  delete logger_;
  logger_ = nullptr;
}

void Logger::SetSTDOutConfig(const STDOutConfig& config)
{
  stdout_config_ = config;
  if(logger_out_.is_open())
  {
    logger_out_.close();
  }
}

void Logger::SetSTDOutConfig(const STDOutMode& mode, const std::string& path)
{
  STDOutConfig config;
  config.mode = mode;
  config.path = path;
  SetSTDOutConfig(config);
}

void Logger::PrintMessage(const std::string& message)
{
  const std::string kFormatTimeMessage = "%m-%d-%Y %H:%M:%S ";
  const int kSizeStringTimeMessage = 21;
  time_t time_message = time(NULL);
  char time_message_string[kSizeStringTimeMessage];
  strftime(time_message_string, kSizeStringTimeMessage, kFormatTimeMessage.c_str(), localtime(&time_message));
  if(!logger_out_.is_open())
  {
    logger_out_.open(stdout_config_.path, stdout_config_.mode == STDOutMode::kSTDOutModeTruncate ? std::ios::trunc : std::ios::app);
  }
  logger_out_ << time_message_string << message << std::endl;
}
