#include <string>
#include <vector>
#include "stdout_config.h"

using std::string;
using std::vector;

struct DataProcess
{
  string name;
  string executable_path;
  STDOutConfig stdout_config;
  vector<string> cmd_arguments;
};
