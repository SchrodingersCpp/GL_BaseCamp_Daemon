#include "logger.h"
#include <ctime>

using std::string;
using std::ofstream;
using std::ios;
using std::ios_base;
using std::endl;

Logger* Logger::logger_ = nullptr;

Logger::Logger()
{
  const string kLogDefaultPathFile = "./log.txt";
  stdout_config_ = new STDOutConfig;
  logger_out_ = new ofstream;
  SetSTDOutConfig(kSTDOutModeTruncate, kLogDefaultPathFile);
}

Logger::~Logger()
{
  delete stdout_config_;
  if(logger_out_->is_open())
  {
    logger_out_->close();
  }
  delete logger_out_;
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
  *stdout_config_ = config;
  if(logger_out_->is_open())
  {
    logger_out_->close();
  }
}

void Logger::SetSTDOutConfig(const STDOutMode& mode, const string& path)
{
  STDOutConfig config;
  config.mode = mode;
  config.path = path;
  SetSTDOutConfig(config);
}

void Logger::PrintMessage(const string& message)
{
  const string kFormatTimeMessage = "%m-%d-%Y %H:%M:%S ";
  const int kSizeStringTimeMessage = 21;
  time_t time_message = time(NULL);
  char time_message_string[kSizeStringTimeMessage];
  strftime(time_message_string, kSizeStringTimeMessage, kFormatTimeMessage.c_str(), localtime(&time_message));
  if(!logger_out_->is_open())
  {
    logger_out_->open(stdout_config_->path, stdout_config_->mode == kSTDOutModeTruncate ? ios::trunc : ios::app);
  }
  *logger_out_ << time_message_string << message << endl;
}
