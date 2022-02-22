#include "parser.h"
#include "logger.h"

#include <fstream>
#include <algorithm>

Logger* logger = Logger::GetLogger();

/******** public members *********/

Parser::Parser()
{
  valid_yaml_flags_ = {
    {YamlFlags::kFlagProcess,      "processes"},
    {YamlFlags::kFlagName,         "name"},
    {YamlFlags::kFlagExePath,      "executable-path"},
    {YamlFlags::kFlagStdoutConfig, "stdout-config"},
    {YamlFlags::kFlagFile,         "file"},
    {YamlFlags::kFlagMode,         "mode"},
    {YamlFlags::kFlagCmdArgs,      "cmd-arguments"},
    {YamlFlags::kFlagOptionName,   "option-name"},
  };
}

void Parser::SetFilePath(const std::string& path) { yaml_file_path_ = path; }

std::vector<DataProcess> Parser::GetProcessData()
{
  if (ReadYAMLFile())
  {
    FillProcs();
  }
  return procs_;
}

/******** end of public members ********/

/******** private members ********/

bool Parser::ReadYAMLFile()
{
  logger->PrintMessage("Reading YAML file...");
  // open YAML-file
  std::ifstream yaml_file{yaml_file_path_};
  // check if YAML-file exists
  if (!yaml_file.is_open())
  {
    logger->PrintMessage("File not found!");
    return false;
  }
  // read YAML-file content
  std::string yaml_line{};
  while (std::getline(yaml_file, yaml_line))
  {
    if (!EmptyOrWhitespaceLine(yaml_line)) { yaml_content_.push_back(yaml_line); }
  }
  // close YAML-file
  yaml_file.close();
  // check if YAML-content is valid
  if (!YAMLValidityCheck()) { return false; }
  return true;
}

// check if line is empty or consists of space and tab chars only
bool Parser::EmptyOrWhitespaceLine(const std::string& line)
{
  const std::string whitespace{" \t"};
  const auto iBegin{line.find_first_not_of(whitespace)};
  const bool isEmpty{iBegin >= line.size()};
  return isEmpty;
}

bool Parser::YAMLValidityCheck()
{
  logger->PrintMessage("YAML content validity check...");
  for (const std::string& line : yaml_content_)
  {
    if (!SingleColonCheck(line)) { return false; }
    AddFlagNameValuePair(line); // vectorize YAML-content
  }
  if (!ProcessFlagCheck()) { return false; }
  if (!ValidFlagsCheck()) { return false; }
  std::vector<int> flagNamePos; // 'name' flag indices
  if (!GetFlagNameIndices(flagNamePos)) { return false; };
  if (!SingleFlagPerProcCheck(flagNamePos)) { return false; };
  if (!ModeValueCheck()) { return false; };
  return true;
}

// check if line contains only one colon char
bool Parser::SingleColonCheck(const std::string& line)
{
  // count colon char occurrences in line
  auto cnt{std::count(line.begin(), line.end(), ':')};
  if (cnt != 1)
  {
    logger->PrintMessage("Only one colon is expected!");
    return false;
  }
  return true;
}

// get flag name and value from line
void Parser::AddFlagNameValuePair(const std::string& line)
{
  std::string flagName{GetFlagName(line)};
  std::string flagValue{GetFlagValue(line)};
  yaml_flag_value_.push_back(std::make_pair(flagName, flagValue));
}

bool Parser::ProcessFlagCheck()
{
  // check if YAML-file begins with "processes" flag
  if (yaml_flag_value_[0].first != valid_yaml_flags_[YamlFlags::kFlagProcess])
  {
    logger->PrintMessage("YAML file should start with 'processes' key!");
    return false;
  }
  // delete first line since it is not needed anymore
  yaml_flag_value_.erase(yaml_flag_value_.begin());
  // check if "processes" occurs only once
  for (const auto& flagValuePair : yaml_flag_value_)
  {
    if (flagValuePair.first == valid_yaml_flags_[YamlFlags::kFlagProcess])
    {
      logger->PrintMessage("Single occurrence of 'processes' flag is expected!");
      return false;
    }
  }
  return true;
}

// get flag name from line
std::string Parser::GetFlagName(const std::string& line)
{
  int iBegin{GetFlagNameBegin(line)};
  int iEnd{GetFlagNameEnd(line)};
  return line.substr(iBegin, iEnd - iBegin + 1);
}

// get flag value from line
std::string Parser::GetFlagValue(const std::string& line)
{
  int iBegin{GetFlagNameEnd(line) + 1}; // colon position
  std::string flagValue{line.substr(iBegin)};
  // trim trailing whitespace chars
  const std::string whitespace{" "};
  int iEnd{static_cast<int>(flagValue.find_last_not_of(whitespace))};
  flagValue = flagValue.substr(0, iEnd + 1);
  if (flagValue.size() == 1) // if flagValue == ":"
  {
    flagValue = "";
  }
  else
  {
    // trim leading whitespace chars (exclude first colon char)
    iBegin = static_cast<int>(flagValue.find_first_not_of(whitespace, 1));
    flagValue = flagValue.substr(iBegin);
  }
  return flagValue;
}

// flag name first char position in line
int Parser::GetFlagNameBegin(const std::string& line)
{
  const std::string tabChars{" -"};
  int flagBegin{};
  flagBegin = static_cast<int>(line.find_first_not_of(tabChars));
  return flagBegin;
}

// flag name last char position in line
int Parser::GetFlagNameEnd(const std::string& line)
{
  return static_cast<int>(line.find(':')) - 1;
}

// check if all flags are valid ones
bool Parser::ValidFlagsCheck()
{
  for (const auto& keyValuePair : yaml_flag_value_)
  {
    bool isValidFlag{false};
    for (const auto& validKeyValue : valid_yaml_flags_)
    {
      if (keyValuePair.first == validKeyValue.second)
      {
        isValidFlag = true;
        break;
      }
    }
    if (!isValidFlag)
    {
      logger->PrintMessage("Invalid flag!");
      return false;
    }
  }
  return true;
}

// get 'name' flag indices to separate process data
bool Parser::GetFlagNameIndices(std::vector<int>& flagNamePos)
{
  for (int i{0}; i < yaml_flag_value_.size(); ++i)
  {
    if (yaml_flag_value_[i].first == valid_yaml_flags_[YamlFlags::kFlagName])
    {
      flagNamePos.push_back(i);
    }
  }
  // check first 'name' flag occurrence after 'processes' flag
  if (flagNamePos[0] != 0)
  {
    logger->PrintMessage("'name' flag expected after 'processes'!");
    return false;
  }
  return true;
}

// check allowed number of flags per process
bool Parser::SingleFlagPerProcCheck(std::vector<int>& flagNamePos)
{
  flagNamePos.push_back(static_cast<int>(yaml_flag_value_.size()));
  for (int i{0}; i < flagNamePos.size(); ++i)
  {
    // fill single process flags slice
    std::vector<std::string> flagSlice;
    for (int j{flagNamePos[i]}; j < flagNamePos[i + 1]; ++j)
    {
      flagSlice.push_back(yaml_flag_value_[j].first);
    }
    if (!FlagOccurences(flagSlice))
    {
      logger->PrintMessage("Redundant flags!");
      return false;
    }
  }
  return true;
}

// check flag occurrences in single process
bool Parser::FlagOccurences(const std::vector<std::string>& flagSlice)
{
  bool okOccur{true};
  for (const auto& flag : flagSlice)
  {
    int nOccur{static_cast<int>(std::count(flagSlice.begin(), flagSlice.end(), flag))};
    if (flag != valid_yaml_flags_[YamlFlags::kFlagOptionName])
    {
      if (nOccur != 1) { okOccur = false; }
    }
  }
  return okOccur;
}

// check if "mode" flag value is valid
bool Parser::ModeValueCheck()
{
  for (const auto& keyValuePair : yaml_flag_value_)
  {
    std::string key{keyValuePair.first};
    std::string value{keyValuePair.second};
    if (key == valid_yaml_flags_[YamlFlags::kFlagMode])
    {
      if (((value != "append") && (value != "truncate")))
      {
        logger->PrintMessage("Invalid 'mode' flag!");
        return false;
      }
    }
  }
  return true;
}

void Parser::FillProcs()
{
  logger->PrintMessage("Filling process objects...");
  DataProcess dataProc;
  bool addProc{false}; // add process to vector
  for (const auto& keyValuePair : yaml_flag_value_)
  {
    std::string flag{keyValuePair.first};
    std::string value{keyValuePair.second};
    if (flag == valid_yaml_flags_[YamlFlags::kFlagName])
    {
      // prevent adding empty process at first "name" occurrence
      if (addProc) { procs_.push_back(dataProc); }
      dataProc = {};
      dataProc.name = value;
      addProc = true;
    }
    else if (flag == valid_yaml_flags_[YamlFlags::kFlagExePath])
    {
      dataProc.executable_path = value;
    }
    else if (flag == valid_yaml_flags_[YamlFlags::kFlagFile])
    {
      dataProc.stdout_config.path = value;
    }
    else if (flag == valid_yaml_flags_[YamlFlags::kFlagMode])
    {
      if (value == "truncate")
      {
        dataProc.stdout_config.mode = STDOutMode::kSTDOutModeTruncate;
      }
      else if (value == "append")
      {
        dataProc.stdout_config.mode = STDOutMode::kSTDOutModeAppend;
      }
    }
    else if (flag == valid_yaml_flags_[YamlFlags::kFlagOptionName])
    {
      dataProc.cmd_arguments.push_back(value);
    }
  }
  // add last process
  procs_.push_back(dataProc);
}

/******** end of private members ********/
