#include "logger.h"
#include <iostream>

using std::cout;
using std::endl;

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
  cout << message << endl;
}
