#include "parser.h"

#include <fstream>
#include <cassert>

/******** public members *********/

Parser::Parser(): yaml_file_path_{}, yaml_content_{}, procs_{}
{
  valid_flags_[static_cast<int>(yamlFlags::kFlagProcess)]      = "processes:";
  valid_flags_[static_cast<int>(yamlFlags::kFlagName)]         = "- name:";
  valid_flags_[static_cast<int>(yamlFlags::kFlagExePath)]      = "    executable-path:";
  valid_flags_[static_cast<int>(yamlFlags::kFlagStdoutConfig)] = "    stdout-config:";
  valid_flags_[static_cast<int>(yamlFlags::kFlagFile)]         = "        file:";
  valid_flags_[static_cast<int>(yamlFlags::kFlagMode)]         = "        mode:";
  valid_flags_[static_cast<int>(yamlFlags::kFlagCmdArgs)]      = "    cmd-arguments:";
  valid_flags_[static_cast<int>(yamlFlags::kFlagOptionName)]   = "        - option-name:";
}

void Parser::SetFilePath(const std::string& path) { yaml_file_path_ = path; }

std::vector<DataProcess>* Parser::GetProcessData()
{
  ReadYAMLFile();
  FillProcesses();
  if (procs_.empty()) { return nullptr; }
  return &procs_;
}

/******** end of public members ********/

/******** private members ********/

void Parser::ReadYAMLFile()
{
  // open YAML-file
  std::ifstream yaml_file{yaml_file_path_};
  // check if YAML-file exists
  assert((yaml_file.is_open() == true) && "File not found!");
  // read YAML-file content
  std::string yaml_line{};
  while (std::getline(yaml_file, yaml_line))
  {
    if (!yaml_line.empty()) { yaml_content_.push_back(yaml_line); }
  }
  // close YAML-file
  yaml_file.close();
  // check if YAML-content is valid
  YAMLValidityCheck();
}

void Parser::YAMLValidityCheck()
{
  // check if YAML-file content starts with "processes" flag
  assert((yaml_content_[0].find(
        valid_flags_[static_cast<int>(yamlFlags::kFlagProcess)]) !=
        std::string::npos) && "YAML should start with 'processes:' key!");
  // check if rest of YAML-file content has valid flags
  for (auto& line : yaml_content_)
  {
    bool isValidFlag{false};
    for (auto& flag : valid_flags_)
    {
      if (line.find(flag) != std::string::npos)
      {
        isValidFlag = true;
        break;
      }
    }
    assert(isValidFlag == true && "YAML contains invalid flag!");
  }
}

void Parser::FillProcesses()
{
  for (auto& line : yaml_content_)
  {
    for (auto& flag : valid_flags_)
    {
      // check if line contains flag
      const auto flagFound{line.find(flag)};
      if (flagFound != std::string::npos)
      {
        // create process struct at each "Name" flag
        if (flag == valid_flags_[static_cast<int>(yamlFlags::kFlagName)])
        {
          procs_.push_back(DataProcess{});
        }
        if ((flag !=
             valid_flags_[static_cast<int>(yamlFlags::kFlagProcess)]) &&
            (flag !=
             valid_flags_[static_cast<int>(yamlFlags::kFlagStdoutConfig)]) &&
            (flag !=
             valid_flags_[static_cast<int>(yamlFlags::kFlagCmdArgs)]))
        {
          std::string flagValue{};
          GetFlagValue(flag, line, flagValue);
          // create and/or fill struct for single process
          FillProcessStruct(flag, flagValue);
          break;
        }
      }
    }
  }
}

void Parser::GetFlagValue(std::string& flag, std::string& line,
                          std::string& flagValue)
{
  // skip flag portion from line string
  const auto i{line.find(flag) + flag.length()};
  std::string value{line.substr(i)};
  // trim leading and trailing whitespaces (if any)
  const char whitespace[]{" \t"};
  const auto iBegin{value.find_first_not_of(whitespace)};
  const auto iFinal{value.find_last_not_of(whitespace)};
  flagValue = value.substr(iBegin, iFinal - iBegin + 1);
}

void Parser::FillProcessStruct(std::string& flag, std::string& flagValue)
{
  if (flag == valid_flags_[static_cast<int>(yamlFlags::kFlagName)])
  {
    procs_.back().name = flagValue;
  }
  else if (flag == valid_flags_[static_cast<int>(yamlFlags::kFlagExePath)])
  {
    procs_.back().executable_path = flagValue;
  }
  else if (flag == valid_flags_[static_cast<int>(yamlFlags::kFlagFile)])
  {
    procs_.back().stdout_config.path = flagValue;
  }
  else if (flag == valid_flags_[static_cast<int>(yamlFlags::kFlagMode)])
  {
    procs_.back().stdout_config.mode =
        static_cast<STDOutMode>(flagValue == "append");
  }
  else if (flag == valid_flags_[static_cast<int>(yamlFlags::kFlagOptionName)])
  {
    procs_.back().cmd_arguments.push_back(flagValue);
  }
}

/******** end of private members ********/
