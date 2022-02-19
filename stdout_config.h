#ifndef STDOUT_CONFIG_H
#define STDOUT_CONFIG_H

#include <string>

enum class STDOutMode
{
  kSTDOutModeTruncate = 0,
  kSTDOutModeAppend
};

struct STDOutConfig
{
  std::string path;
  STDOutMode mode;
};

#endif