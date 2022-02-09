#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream> 

#include <iprocess_launcher.h>

class ProcessLauncher : public IProcessLauncher
{
private:
    vector<Process>& processes;

public:

    ProcessLauncher(vector<Process>& proc) : processes(proc) {}

    bool isPathExist(string path) {
        std::ifstream file(path);
        bool result = file.is_open();
        file.close();
        return result;
    }

    void printError(string processName, string text) {
        std::cout << "##### Error run " << processName << " process #####" << std::endl; 
        std::cout << text << std::endl; 
    }


    void SetProcessData(vector<Process>& processes) {

        for(auto process: processes) {
            
            if (!isPathExist(process.getExePath())) {
                printError(process.getName(), "Executable-path: file doesn't exist.");    
                continue;
            }

            if (!isPathExist(process.getStdoutConfigFile())) {
                printError(process.getName(), "stdout-config: file doesn't exist.");
                continue;
            }
            
            string mode = process.getStdoutConfigMode();

            if (mode.compare("truncate") != 0 && mode.compare("append") != 0) {
                printError(process.getName(), "stdout-config: file mode isn't correct (must be truncate or append).");
                continue;
            }

            std::cout << "Run " << process.getName() << " process" << std::endl;

            string command(process.getExePath());

            for (auto arg: process.getCmdArgs()) {
                command.append(" ");
                command.append(arg);
            }
            
            if (mode.compare("append") == 0) {
                command.append(" >> ");
            } else {
                command.append(" > ");
            }
            
            command.append(process.getStdoutConfigFile());

            int status = system(command.c_str());

            if (status < 0)
                std::cout << "Error: " << std::strerror(errno) << '\n';
            else
            {
                if (WIFEXITED(status))
                    std::cout << "Program " << process.getName() << " returned normally, exit code " << WEXITSTATUS(status) << '\n';
                else
                    std::cout << "Program " << process.getName() << " exited abnormaly\n";
            }   
        }
    }
};

