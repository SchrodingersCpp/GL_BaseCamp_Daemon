#include "logger.h"
#include <iostream>
#include <ctime>

using std::cout;
using std::endl;

const char* kFormatTimeMessage = "%m-%d-%Y %H:%M:%S ";
const int kSizeStringTimeMessage = 21;

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
  strftime(time_message_string, kSizeStringTimeMessage, kFormatTimeMessage, localtime(&time_message));
  cout << time_message_string << message << endl;
}
