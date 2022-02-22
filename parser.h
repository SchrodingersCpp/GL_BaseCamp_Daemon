#ifndef PARSER_H
#define PARSER_H

#include "iparser.h"

#include <string>
#include <vector>
#include <map>

class Parser: public IParser
{
 public:
  Parser();
  
  void SetFilePath(const std::string& path);
  std::vector<DataProcess> GetProcessData();
  
 private:
  enum class YamlFlags
  {
    kFlagProcess = 0,
    kFlagName,
    kFlagExePath,
    kFlagStdoutConfig,
    kFlagFile,
    kFlagMode,
    kFlagCmdArgs,
    kFlagOptionName,
    kTotal,
  };
  std::map<YamlFlags, std::string> valid_yaml_flags_;
  std::string yaml_file_path_;
  std::vector<std::string> yaml_content_;
  std::vector<std::pair<std::string, std::string>> yaml_flag_value_;
  std::vector<DataProcess> procs_;
  
  bool ReadYAMLFile();
  bool EmptyOrWhitespaceLine(const std::string& line);
  bool YAMLValidityCheck();
  bool SingleColonCheck(const std::string& line);
  void AddFlagNameValuePair(const std::string& line);
  bool ProcessFlagCheck();
  std::string GetFlagName(const std::string& line);
  std::string GetFlagValue(const std::string& line);
  int GetFlagNameBegin(const std::string& line);
  int GetFlagNameEnd(const std::string& line);
  bool ValidFlagsCheck();
  bool GetFlagNameIndices(std::vector<int>& flagNamePos);
  bool SingleFlagPerProcCheck(std::vector<int>& flagNamePos);
  bool FlagOccurences(const std::vector<std::string>& flagSlice);
  bool ModeValueCheck();
  void FillProcs();
};

#endif // PARSER_H
