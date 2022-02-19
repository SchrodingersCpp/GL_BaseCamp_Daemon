#include <iostream>
#include <fstream>
#include <cstring>
#include "process_launcher.h"


bool ProcessLauncher::isPathExist(std::string path) {
    std::ifstream file(path);
    bool result = file.is_open();
    file.close();
    return result;
}

void ProcessLauncher::printError(std::string processName, std::string text) {
    std::cout << "##### Error run " << processName << " process #####" << std::endl; 
    std::cout << text << std::endl; 
}


void ProcessLauncher::SetProcessData(std::vector<DataProcess>& processes) {

    for(const auto &process: processes) {
        
        if (!isPathExist(process.executable_path)) {
            printError(process.name, "Executable-path: file doesn't exist.");    
            continue;
        }

        if (!isPathExist(process.stdout_config.path)) {
            printError(process.name, "stdout-config: file doesn't exist.");
            continue;
        }

        std::string command("setsid " + process.executable_path);

        for (auto arg: process.cmd_arguments) {
            command.append(" ");
            command.append(arg);
        }
        
        STDOutMode mode = process.stdout_config.mode;
        
        (mode == kSTDOutModeAppend) ? command.append(" >> ") : command.append(" > ");
        
        command.append(process.stdout_config.path);

        std::cout << "Run " << process.name << " process" << std::endl;
        int status = system(command.c_str());

        if (status < 0)
            std::cout << "Error: " << std::strerror(errno) << '\n';
        else
        {
            if (WIFEXITED(status))
                std::cout << "Program " << process.name << " returned normally, exit code " << WEXITSTATUS(status) << '\n';
            else
                std::cout << "Program " << process.name << " exited abnormaly\n";
        }   
    }
}


