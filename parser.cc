#include "parser.h"

#include <fstream>
#include <cassert>

Parser::Parser():yaml_file_path_{}, yaml_content_{}, procs_{}
{
  yaml_flags_ = {{"flagProcesses",    "processes:"},
                 {"flagName",         " name:"},
                 {"flagExePath",      " executable-path:"},
                 {"flagStdoutConfig", " stdout-config:"},
                 {"flagFile",         " file:"},
                 {"flagMode",         " mode:"},
                 {"flagCmdArgs",      " cmd-arguments:"},
                 {"flagOptionName",   " option-name:"}
                };
}

void Parser::SetFilePath(string& path)
{
  yaml_file_path_ = path;
}

vector<DataProcess>* Parser::GetProcessData()
{
  ReadYAML();
  FillProcesses();
  
  if (procs_.size() == 0)
  {
    return nullptr;
  }
  
  return &procs_;
}

void Parser::ReadYAML()
{
  // open YAML-file
  std::ifstream yaml_file{yaml_file_path_};
  
  // check if YAML-file exists
  assert((yaml_file.is_open() == true) && "File not found!");
  
  // read YAML-file content
  string yaml_line{};
  while (std::getline(yaml_file, yaml_line))
  {
    if (yaml_line != "") // exclude empty lines
    {
      yaml_content_.push_back(yaml_line);
    }
  }
  
  // close YAML-file
  yaml_file.close();
  
  // check if YAML-content is valid
  ValidYAML();
}

// check if YAML-file content is valid
void Parser::ValidYAML()
{
  // check if YAML-file content starts with "processes" flag
  assert((yaml_content_[0].find(yaml_flags_["flagProcesses"]) != string::npos)
         && "YAML should start with 'processes:' key!");
  
  // delete "processes" flag line (not needed for further processing)
  yaml_content_.erase(yaml_content_.begin());
  yaml_flags_.erase("flagProcesses");
  
  // check if rest of YAML-file content has valid flags
  for (auto& line : yaml_content_)
  {
    bool validFlag{false};
    for (auto& flag : yaml_flags_)
    {
      if (line.find(flag.second) != string::npos)
      {
        validFlag = true;
        break;
      }
    }
    assert(validFlag == true && "YAML contains invalid flag!");
  }
}

void Parser::FillProcesses()
{
  // delete unnecessary flags without values
  yaml_flags_.erase("flagStdoutConfig");
  yaml_flags_.erase("flagCmdArgs");
  
  for (auto& line : yaml_content_)
  {
    for (auto& flag : yaml_flags_)
    {
      // check if line contains flag
      const auto flagFound{line.find(flag.second)};
      
      if (flagFound != string::npos)
      {
        // create process struct at each "Name" flag
        if (flag.first == "flagName") {procs_.push_back(DataProcess{});}
        
        string flagValue{};
        GetFlagValue(flag.second, line, flagValue);
        
        // create and/or fill struct for single process
        FillProcessStruct(flag.second, flagValue);
        break;
      }
    }
  }
}

void Parser::GetFlagValue(string& flag, string& line, string& flagValue)
{
  // skip flag portion from line string
  const auto i{line.find(flag) + flag.length()};
  string value{line.substr(i)};
  
  // trim leading and trailing whitespaces (if any)
  const char whitespace[]{" \t"};
  const auto iBegin{value.find_first_not_of(whitespace)};
  const auto iFinal{value.find_last_not_of(whitespace)};
  flagValue = value.substr(iBegin, iFinal - iBegin + 1);
}

// create and/or fill struct for single process
void Parser::FillProcessStruct(string& flag, string& flagValue)
{
  if (flag == yaml_flags_["flagName"])
  {
    procs_.back().name = flagValue;
    return;
  }
  else if (flag == yaml_flags_["flagExePath"])
  {
    procs_.back().executable_path = flagValue;
    return;
  }
  else if (flag == yaml_flags_["flagFile"])
  {
    procs_.back().stdout_config.path = flagValue;
    return;
  }
  else if (flag == yaml_flags_["flagMode"])
  {
    procs_.back().stdout_config.mode =
        static_cast<STDOutMode>(flagValue == "append");
    return;
  }
  else if (flag == yaml_flags_["flagOptionName"])
  {
    procs_.back().cmd_arguments.push_back(flagValue);
    return;
  }
}
