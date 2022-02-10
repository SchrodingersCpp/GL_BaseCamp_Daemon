#ifndef PARSER_H
#define PARSER_H

#include "iparser.h"

#include <string>
#include <vector>

#include <fstream>
#include <cassert>

using std::string;
using std::vector;
using vecstr = vector<string>;

class Parser: public IParser {
 public:
  void SetFilePath(string& path);
  vector<DataProcess>* GetProcessData();
void test();
  
 private:
  string yamlFilePath;
  vecstr yamlContent;
  vector<DataProcess> procs;
  
  void readYAML();
};

#endif // PARSER_H
