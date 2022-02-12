#include "logger.h"
#include <fstream>
#include <ctime>

using std::ofstream;
using std::ios;
using std::ios_base;
using std::endl;

const char* kFormatTimeMessage = "%m-%d-%Y %H:%M:%S ";
const int kSizeStringTimeMessage = 21;
const char* kLogDefaultPathFile = "./log.txt";

Logger* Logger::logger_ = nullptr;

Logger::Logger()
{
  stdout_config_ = new STDOutConfig;
  stdout_config_->mode = kSTDOutModeTruncate;
  stdout_config_->path = string(kLogDefaultPathFile);
  first_file_open_flag_ = true;
}

Logger* Logger::GetLogger(void)
{
  if(logger_ == nullptr)
  {
    logger_ = new Logger;
  }
  return logger_;
}

void Logger::SetSTDOutConfig(STDOutConfig& config)
{
  *stdout_config_ = config;
  first_file_open_flag_ = true;
}

void Logger::PrintMessage(string& message)
{
  time_t time_message = time(NULL);
  char time_message_string[kSizeStringTimeMessage];
  ofstream logger_out;
  ios_base::openmode logger_out_mode;
  logger_out_mode = (stdout_config_->mode == kSTDOutModeTruncate && first_file_open_flag_) ? ios::trunc : ios::app;
  strftime(time_message_string, kSizeStringTimeMessage, kFormatTimeMessage, localtime(&time_message));
  logger_out.open(stdout_config_->path, logger_out_mode);
  if(logger_out.is_open())
  {
    logger_out << time_message_string << message << endl;
    first_file_open_flag_ = false;
  }
  logger_out.close();
}
