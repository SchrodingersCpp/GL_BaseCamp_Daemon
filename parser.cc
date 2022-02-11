#include <iostream> // TO BE DELETED TO BE DELETED TO BE DELETED TO BE DELETED TO BE DELETED

#include "parser.h"

#include <fstream>
#include <cassert>

Parser::Parser():yaml_file_path_{}, yaml_content_{}, procs_{}
{
  SetYAMLFlags();
}

void Parser::SetYAMLFlags()
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
  for (auto& line : yaml_content_) {
    bool validFlag{false};
    for (auto& flag : yaml_flags_) {
      if (line.find(flag.second) != string::npos) {
        validFlag = true;
        break;
      }
    }
    assert(validFlag == true && "YAML contains invalid flag!");
  }
}

void Parser::TEST() // TO BE DELETED TO BE DELETED TO BE DELETED TO BE DELETED TO BE DELETED
{
  std::cout << "TESTING\n-------\n";
  for (string line : yaml_content_)
  {
    std::cout << line << '\n';
  }
  std::cout << "\n\n";
  for (auto item : yaml_flags_)
  {
    std::cout << item.first << "\t\t" << item.second << '\n';
  }
  std::cout << "\n\n";
}

int main()
{
  Parser p{};
  string yaml{"/home/linux/Documents/config.yaml"};
  p.SetFilePath(yaml);
  p.GetProcessData();
  
p.TEST(); // TO BE DELETED TO BE DELETED TO BE DELETED TO BE DELETED TO BE DELETED
  
  return 0;
}
