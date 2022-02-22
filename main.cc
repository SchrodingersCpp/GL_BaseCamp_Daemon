#include <vector>
#include <string>
#include "parser.h"
#include "process_launcher.h"
#include "logger.h"
#include "data_process.h"

std::string kProcessConfigurationFilePath = "./config.yaml";
std::string kLoggerFilePath = "log.txt";
STDOutMode kLoggerMode = STDOutMode::kSTDOutModeAppend;

int main()
{
  std::vector<DataProcess> processes;
  Parser yaml_parser;
  ProcessLauncher process_launcher;
  Logger* logger = Logger::GetLogger();
  logger->SetSTDOutConfig(kLoggerMode, kLogFilePath);
  logger->PrintMessage("Start application.");
  logger->PrintMessage("Startup YAML parser.");
  yaml_parser.SetFilePath(kProcessConfigurationFilePath);
  processes = yaml_parser.GetProcessData();
  if(processes.empty())
  {
    logger->PrintMessage("Processes data is not finded.");
    Logger::DeleteLogger();
    return 1;
  }
  else
  {
    logger->PrintMessage("Startup processes.");
    process_launcher.SetProcessData(processes);
    logger->PrintMessage("Stop application.");
    Logger::DeleteLogger();
    return 0;
  }
}
