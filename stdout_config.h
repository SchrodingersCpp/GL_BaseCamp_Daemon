#include <string>

enum STDOutMode
{
  kSTDOutModeTruncate = 0,
  kSTDOutModeAppend
};

struct STDOutConfig
{
  std::string path;
  STDOutMode mode;
};
