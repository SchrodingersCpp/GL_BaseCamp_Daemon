#include <string>

using std::string;

enum STDOutMode
{
  kSTDOutModeTruncate = 0,
  kSTDOutModeAppend
};

struct STDOutConfig
{
  string path;
  STDOutMode mode;
};
