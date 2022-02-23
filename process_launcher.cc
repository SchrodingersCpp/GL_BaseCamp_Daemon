#include <unistd.h>
#include <sys/wait.h>
#include <spawn.h>
#include <fcntl.h>

#include <fstream>
#include <cstring>

#include "process_launcher.h"
#include "logger.h"

bool ProcessLauncher::IsPathExist(const std::string &path)
{
    std::ifstream file(path);
    bool result = file.is_open();
    file.close();
    return result;
}

void ProcessLauncher::SetProcessData(const std::vector<DataProcess> &processes)
{
    Logger *logger = Logger::GetLogger();
    extern char **environ;
    for (const auto &process : processes)
    {
        if (!IsPathExist(process.executable_path))
        {
            logger->PrintMessage("Error run " + process.name + ". Executable-path: file doesn't exist.");
            continue;
        }
        if (!IsPathExist(process.stdout_config.path))
        {
            logger->PrintMessage("Error run " + process.name + ". Stdout-config: file doesn't exist.");
            continue;
        }
        pid_t pid;
        const char **argv = new const char *[process.cmd_arguments.size() + 2]; // + 2 = program_name, NULL
        argv[0] = process.name.c_str();
        // Copy args
        for (size_t j = 0; j < process.cmd_arguments.size() + 1; ++j)
            argv[j + 1] = process.cmd_arguments[j].c_str();
        // End of arguments sentinel is NULL
        argv[process.cmd_arguments.size() + 1] = NULL;
        posix_spawn_file_actions_t action;
        int err = posix_spawn_file_actions_init(&action);
        if (err != 0)
        {
            logger->PrintMessage("Error: posix_spawn_file_actions_init");
            continue;
        }
        STDOutMode mode = process.stdout_config.mode;
        const char *log_file = process.stdout_config.path.c_str();
        if (mode == STDOutMode::kSTDOutModeAppend)
        {
            err = posix_spawn_file_actions_addopen(&action, 1, log_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else
        {
            err = posix_spawn_file_actions_addopen(&action, 1, log_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }

        if (err != 0)
        {
            logger->PrintMessage("Error: posix_spawn_file_actions_addopen");
            continue;
        }
        logger->PrintMessage("Run " + process.name + " process.");
        int status = posix_spawn(&pid, process.executable_path.c_str(), &action, NULL, const_cast<char **>(argv), environ);
        if (status == 0)
        {
            do
            {
                if (waitpid(pid, &status, 0) != -1)
                {
                    logger->PrintMessage("Process " + process.name + " success finished.");
                }
                else
                {
                    logger->PrintMessage("Error: waitpid.");
                }
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        else
        {
            std::string error(strerror(status));
            logger->PrintMessage("Error posix_spawn: " + error);
        }
    }
}