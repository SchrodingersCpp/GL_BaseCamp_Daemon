#ifndef PARSER_H
#define PARSER_H

#include "iparser.h"

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using vecstr = vector<string>;
using dict = std::map<string, string>;

class Parser: public IParser
{
 public:
  Parser();
  
  void SetFilePath(string& path);
  vector<DataProcess>* GetProcessData();
void TEST();// TO BE DELETED TO BE DELETED TO BE DELETED TO BE DELETED TO BE DELETED
  
 private:
  dict yaml_flags_;
  string yaml_file_path_;
  vecstr yaml_content_;
  vector<DataProcess> procs_;
  
  void SetYAMLFlags();
  void ReadYAML();
  void ValidYAML();
};

#endif // PARSER_H
