#include <iostream>

#include "parser.h"

void Parser::SetFilePath(string& path) {
  yamlFilePath = path;
}

vector<DataProcess>* Parser::GetProcessData() {
  readYAML();
  
  return &procs;
}

void Parser::readYAML() {
  // open YAML-file
  std::fstream yamlFile{};
  yamlFile.open(yamlFilePath, std::fstream::in);
  
  // check if YAML-file exists
  assert((yamlFile.is_open() == true) && "File not found!");
  
  // read YAML-file content
  string yamlLine{};
  while (std::getline(yamlFile, yamlLine)) {
    if (yamlLine != "") { // exclude empty lines
      yamlContent.push_back(yamlLine);
    }
  }
  
  // close YAML-file
  yamlFile.close();
}

void Parser::test() {
  std::cout << "TESTING\n-------\n";
  for (string line : yamlContent) {
    std::cout << line << '\n';
  }
}

int main() {
  Parser p{};
  string yaml{"/home/linux/Documents/config.yaml"};
  p.SetFilePath(yaml);
  p.GetProcessData();
  
p.test();
  
  return 0;
}
