#ifndef PARSER_H
#define PARSER_H

#include "iparser.h"

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using dict = std::map<string, string>;

class Parser: public IParser
{
 public:
  Parser();
  
  void SetFilePath(string& path);
  vector<DataProcess>* GetProcessData();

 private:
  dict yaml_flags_;
  string yaml_file_path_;
  vector<string> yaml_content_;
  vector<DataProcess> procs_;
  
  void ReadYAML();
  void ValidYAML();
  void FillProcesses();
  void GetFlagValue(string& flag, string& line, string& flagValue);
  void FillProcessStruct(string& flag, string& flagValue);
};

#endif // PARSER_H
