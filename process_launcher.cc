#include <iostream>
#include <fstream>
#include <cstring>
#include "process_launcher.h"
#include "logger.h"


bool ProcessLauncher::isPathExist(std::string path) {
    std::ifstream file(path);
    bool result = file.is_open();
    file.close();
    return result;
}


void ProcessLauncher::SetProcessData(std::vector<DataProcess>& processes) {

    Logger* logger = Logger::GetLogger();

    for(const auto &process: processes) {
        
        if (!isPathExist(process.executable_path)) {
            logger->PrintMessage("Error run " + process.name + ". Executable-path: file doesn't exist.");
            continue;
        }

        if (!isPathExist(process.stdout_config.path)) {
            logger->PrintMessage("Error run " + process.name + ". Stdout-config: file doesn't exist.");
            continue;
        }

        std::string command("setsid " + process.executable_path);

        for (auto arg: process.cmd_arguments) {
            command.append(" ");
            command.append(arg);
        }
        
        STDOutMode mode = process.stdout_config.mode;
        
        (mode == STDOutMode::kSTDOutModeAppend) ? command.append(" >> ") : command.append(" > ");
        
        command.append(process.stdout_config.path);

        logger->PrintMessage("Run " + process.name + " process");
        int status = system(command.c_str());
        
        if (status < 0) {
            const char* error = std::strerror(errno);
            std::string error_message(error);
            logger->PrintMessage("Error: " + error_message);
        }
        else
        {
            if (WIFEXITED(status))
                logger->PrintMessage("Program " + process.name + " returned normally, exit code " + std::to_string(WEXITSTATUS(status)));
            else
                logger->PrintMessage("Program " + process.name + " exited abnormaly");
        }   
    }
}


