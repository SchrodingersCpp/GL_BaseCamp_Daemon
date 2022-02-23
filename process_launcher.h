#ifndef PROCESS_LAUNCHER_H
#define PROCESS_LAUNCHER_H

#include "iprocess_launcher.h"

class ProcessLauncher : public IProcessLauncher
{
private:
    bool IsPathExist(const std::string &path);
    char **GetCmdArguments(const std::vector<std::string> cmd_arguments, std::string program_name);
public:
    void SetProcessData(const std::vector<DataProcess>& processes) override;
};

#endif
