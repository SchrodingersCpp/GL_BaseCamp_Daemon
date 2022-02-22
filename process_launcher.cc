#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "process_launcher.h"
#include "logger.h"

bool ProcessLauncher::isPathExist(std::string path) 
{
    std::ifstream file(path);
    bool result = file.is_open();
    file.close();
    return result;
}

void ProcessLauncher::SetProcessData(std::vector<DataProcess>& processes) 
{
    Logger* logger = Logger::GetLogger();
    for(const auto &process: processes) 
    {
        if (!isPathExist(process.executable_path)) 
        {
            logger->PrintMessage("Error run " + process.name + ". Executable-path: file doesn't exist.");
            continue;
        }
        if (!isPathExist(process.stdout_config.path)) 
        {
            logger->PrintMessage("Error run " + process.name + ". Stdout-config: file doesn't exist.");
            continue;
        }
        pid_t pid;
        pid = fork();
        if (pid < 0) 
        {
            // error
            logger->PrintMessage("Fork failed (process " + process.name + ")");
            continue;
        } else if(pid == 0) {
            // child process
            logger->PrintMessage("Run " + process.name + " process.");
            const char **argv = new const char*[process.cmd_arguments.size()+2]; // + 2 = program_name, NULL
            argv[0] = process.name.c_str();
            for (size_t j = 0;  j < process.cmd_arguments.size()+1;  ++j)     // copy args
                argv[j+1] = process.cmd_arguments[j].c_str();
            argv[process.cmd_arguments.size()+1] = NULL;  // end of arguments sentinel is NULL
            STDOutMode mode = process.stdout_config.mode;
            int redirect_fd;
            if (mode == STDOutMode::kSTDOutModeAppend) 
            {
                redirect_fd = open(process.stdout_config.path.c_str(), O_CREAT | O_APPEND | O_WRONLY);
            } 
            else 
            {
                redirect_fd = open(process.stdout_config.path.c_str(), O_CREAT | O_TRUNC | O_WRONLY);
            }
            dup2(redirect_fd, STDOUT_FILENO);
            close(redirect_fd);
            if(execvp(process.executable_path.c_str(), (char**)argv) == -1) 
            {
                fprintf(stderr, "Error executing %s\n", process.executable_path.c_str());
            }
        } 
        else 
        {
            int waitstatus;
            wait(&waitstatus);
            if (WIFEXITED(waitstatus)) 
            {
                logger->PrintMessage("Process " + process.name + " success finished.");
            } 
            else 
            {
                logger->PrintMessage("Process " + process.name + " failed.");
            }     
        }
    }
}


