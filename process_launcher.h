#ifndef PROCESS_LAUNCHER_H
#define PROCESS_LAUNCHER_H

#include "iprocess_launcher.h"

class ProcessLauncher : public IProcessLauncher
{
    
private:
    bool isPathExist(string path);
    void printError(string processName, string text);
public:
    void SetProcessData(vector<DataProcess>& processes) override;
};

#endif