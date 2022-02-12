#include "logger.h"
#include <fstream>
#include <ctime>

using std::ofstream;
using std::ios;
using std::endl;

const char* kFormatTimeMessage = "%m-%d-%Y %H:%M:%S ";
const int kSizeStringTimeMessage = 21;
const char* kLogPathFile = "./log.txt";

Logger* Logger::logger_ = nullptr;

Logger* Logger::GetLogger(void)
{
  if(logger_ == nullptr)
  {
    logger_ = new Logger;
  }
  return logger_;
}

void Logger::PrintMessage(string& message)
{
  time_t time_message = time(NULL);
  char time_message_string[kSizeStringTimeMessage];
  ofstream logger_out;
  strftime(time_message_string, kSizeStringTimeMessage, kFormatTimeMessage, localtime(&time_message));
  logger_out.open(kLogPathFile, ios::app);
  if(logger_out.is_open())
  {
    logger_out << time_message_string << message << endl;
  }
  logger_out.close();
}
