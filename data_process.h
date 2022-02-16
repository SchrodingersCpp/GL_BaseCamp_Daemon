#include <string>
#include <vector>
#include "stdout_config.h"

struct DataProcess
{
  std::string name;
  std::string executable_path;
  STDOutConfig stdout_config;
  std::vector<std::string> cmd_arguments;
};
