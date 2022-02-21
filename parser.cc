#include "parser.h"
#include "logger.h"

#include <fstream>
#include <cassert>
#include <algorithm>

Logger* logger = Logger::GetLogger();

/******** public members *********/

Parser::Parser()
{
  AddValidFlag(yamlFlags_::kFlagProcess,      "processes");
  AddValidFlag(yamlFlags_::kFlagName,         "name");
  AddValidFlag(yamlFlags_::kFlagExePath,      "executable-path");
  AddValidFlag(yamlFlags_::kFlagStdoutConfig, "stdout-config");
  AddValidFlag(yamlFlags_::kFlagFile,         "file");
  AddValidFlag(yamlFlags_::kFlagMode,         "mode");
  AddValidFlag(yamlFlags_::kFlagCmdArgs,      "cmd-arguments");
  AddValidFlag(yamlFlags_::kFlagOptionName,   "option-name");
}

void Parser::SetFilePath(const std::string& path) { yaml_file_path_ = path; }

std::vector<DataProcess> Parser::GetProcessData()
{
  ReadYAMLFile();
  FillProcs();
  return procs_;
}

/******** end of public members ********/

/******** private members ********/

// add valid flag name to list of valid YAML flags
void Parser::AddValidFlag(const yamlFlags_& kFlag, const std::string& flagName)
{
  auto newPair{std::make_pair(kFlag, flagName)};
  valid_yaml_flags_.insert(newPair);
}

void Parser::ReadYAMLFile()
{
  logger->PrintMessage("Reading YAML file...");
  // open YAML-file
  std::ifstream yaml_file{yaml_file_path_};
  // check if YAML-file exists
  assert((yaml_file.is_open() == true) && "File not found!");
  // read YAML-file content
  std::string yaml_line{};
  while (std::getline(yaml_file, yaml_line))
  {
    if (!EmptyOrWhitespaceLine(yaml_line)) { yaml_content_.push_back(yaml_line); }
  }
  // close YAML-file
  yaml_file.close();
  // check if YAML-content is valid
  YAMLValidityCheck();
}

// check if line is empty or consists of space and tab chars only
bool Parser::EmptyOrWhitespaceLine(const std::string& line)
{
  const std::string whitespace{" \t"};
  const auto iBegin{line.find_first_not_of(whitespace)};
  const bool isEmpty{iBegin >= line.size()};
  return isEmpty;
}

void Parser::YAMLValidityCheck()
{
  logger->PrintMessage("YAML content validity check...");
  for (const std::string& line : yaml_content_)
  {
    SingleColonCheck(line);
    AddFlagNameValuePair(line); // vectorize YAML-content
  }
  ProcessFlagCheck();
  ValidFlagsCheck();
  std::vector<int> flagNamePos{}; // 'name' flag indices
  GetFlagNameIndices(flagNamePos);
  SingleFlagPerProcCheck(flagNamePos);
  ModeValueCheck();
}

// check if line contains only one colon char
void Parser::SingleColonCheck(const std::string& line)
{
  // count colon char occurrences in line
  auto cnt{std::count(line.begin(), line.end(), ':')};
  assert((cnt == 1) && "Only one colon is expected!");
}

// get flag name and value from line
void Parser::AddFlagNameValuePair(const std::string& line)
{
  std::string flagName{GetFlagName(line)};
  std::string flagValue{GetFlagValue(line)};
  yaml_flag_value_.push_back(std::make_pair(flagName, flagValue));
}

void Parser::ProcessFlagCheck()
{
  // check if YAML-file begins with "processes" flag
  assert((yaml_flag_value_[0].first ==
          valid_yaml_flags_[yamlFlags_::kFlagProcess]) &&
         "YAML file should start with 'processes' key!");
  // delete first line since it is not needed anymore
  yaml_flag_value_.erase(yaml_flag_value_.begin());
  // check if "processes" occurs only once
  for (const auto& flagValuePair : yaml_flag_value_)
  {
    assert(flagValuePair.first != valid_yaml_flags_[yamlFlags_::kFlagProcess]
           && "Single occurrence of 'processes' flag is expected!");
  }
}

// get flag name from line
std::string Parser::GetFlagName(const std::string& line)
{
  int i0{GetFlagNameBegin(line)};
  int i1{GetFlagNameEnd(line)};
  return line.substr(i0, i1 - i0 + 1);
}

// get flag value from line
std::string Parser::GetFlagValue(const std::string& line)
{
  int i0{GetFlagNameEnd(line) + 1}; // colon position
  std::string flagValue{line.substr(i0)};
  // trim trailing whitespace chars
  const std::string whitespace{" "};
  int i1{static_cast<int>(flagValue.find_last_not_of(whitespace))};
  flagValue = flagValue.substr(0, i1 + 1);
  if (flagValue.size() == 1) // if flagValue == ":"
  {
    flagValue = "";
  }
  else
  {
    // trim leading whitespace chars (exclude first colon char)
    i0 = static_cast<int>(flagValue.find_first_not_of(whitespace, 1));
    flagValue = flagValue.substr(i0);
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
void Parser::ValidFlagsCheck()
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
    assert(isValidFlag && "Invalid flag!");
  }
}

// get 'name' flag indices to separate process data
void Parser::GetFlagNameIndices(std::vector<int>& flagNamePos)
{
  for (int i{0}; i < yaml_flag_value_.size(); ++i)
  {
    if (yaml_flag_value_[i].first == valid_yaml_flags_[yamlFlags_::kFlagName])
    {
      flagNamePos.push_back(i);
    }
  }
  // check first 'name' flag occurrence after 'processes' flag
  assert((flagNamePos[0] == 0) && "'name' flag expected after 'processes'!");
}

// check allowed number of flags per process
void Parser::SingleFlagPerProcCheck(std::vector<int>& flagNamePos)
{
  flagNamePos.push_back(static_cast<int>(yaml_flag_value_.size()));
  for (int i{0}; i < flagNamePos.size(); ++i)
  {
    // fill single process flags slice
    std::vector<std::string> flagSlice{};
    for (int j{flagNamePos[i]}; j < flagNamePos[i + 1]; ++j)
    {
      flagSlice.push_back(yaml_flag_value_[j].first);
    }
    assert(FlagOccurences(flagSlice) && "Redundant flags!");
  }
}

// check flag occurrences in single process
bool Parser::FlagOccurences(const std::vector<std::string>& flagSlice)
{
  bool okOccur{true};
  for (const auto& flag : flagSlice)
  {
    int nOccur{static_cast<int>(std::count(flagSlice.begin(), flagSlice.end(), flag))};
    if (flag != valid_yaml_flags_[yamlFlags_::kFlagOptionName])
    {
      if (nOccur != 1) { okOccur = false; }
    }
  }
  return okOccur;
}

// check if "mode" flag value is valid
void Parser::ModeValueCheck()
{
  for (const auto& keyValuePair : yaml_flag_value_)
  {
    std::string key{keyValuePair.first};
    std::string value{keyValuePair.second};
    if (key == valid_yaml_flags_[yamlFlags_::kFlagMode])
    {
      assert(((value == "append") || (value == "truncate")) &&
             "Invalid 'mode' flag!");
    }
  }
}

void Parser::FillProcs()
{
  logger->PrintMessage("Filling process objects...");
  for (const auto& keyValuePair : yaml_flag_value_)
  {
    std::string flag{keyValuePair.first};
    std::string value{keyValuePair.second};
    if (flag == valid_yaml_flags_[yamlFlags_::kFlagName])
    {
      procs_.push_back(DataProcess());
      procs_.back().name = value;
    }
    else if (flag == valid_yaml_flags_[yamlFlags_::kFlagExePath])
    {
      procs_.back().executable_path = value;
    }
    else if (flag == valid_yaml_flags_[yamlFlags_::kFlagFile])
    {
      procs_.back().stdout_config.path = value;
    }
    else if (flag == valid_yaml_flags_[yamlFlags_::kFlagMode])
    {
      if (value == "truncate")
      {
        procs_.back().stdout_config.mode = STDOutMode::kSTDOutModeTruncate;
      }
      else if (value == "append")
      {
        procs_.back().stdout_config.mode = STDOutMode::kSTDOutModeAppend;
      }
    }
    else if (flag == valid_yaml_flags_[yamlFlags_::kFlagOptionName])
    {
      procs_.back().cmd_arguments.push_back(value);
    }
  }
}

/******** end of private members ********/
