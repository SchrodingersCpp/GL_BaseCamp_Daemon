#ifndef PARSER_H
#define PARSER_H

#include "iparser.h"

#include <string>
#include <vector>

enum class yamlFlags
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

class Parser: public IParser
{
 public:
  Parser();
  
  void SetFilePath(const std::string& path);
  std::vector<DataProcess>* GetProcessData();
  
 private:
  std::string yaml_file_path_;
  std::vector<std::string> yaml_content_;
  std::string valid_flags_[static_cast<int>(yamlFlags::kTotal)];
  std::vector<DataProcess> procs_;
  
  void ReadYAMLFile();
  void YAMLValidityCheck();
  void FillProcesses();
  void GetFlagValue(std::string& flag, std::string& line,
                    std::string& flagValue);
  void FillProcessStruct(std::string& flag, std::string& flagValue);
};

#endif // PARSER_H
