#include <iostream>
#include <vector>

#include "process_launcher.h"

int main() {

    DataProcess process1;
    process1.name = "a";
    process1.executable_path = "./processes/a";
    process1.stdout_config.path = "./logs/example_app1.log";
    process1.stdout_config.mode = STDOutMode::kSTDOutModeAppend;
    std::vector<std::string> cmd_arguments1{"1", "a", "example_app1"};
    process1.cmd_arguments = cmd_arguments1;

    DataProcess process2;
    process2.name = "b";
    process2.executable_path = "./processes/b";
    process2.stdout_config.path = "./logs/example_app2.log";
    process2.stdout_config.mode = STDOutMode::kSTDOutModeTruncate;
    std::vector<std::string> cmd_arguments2{"2", "b", "example_app2"};
    process2.cmd_arguments = cmd_arguments2;

    std::vector<DataProcess> processes{process1, process2};

    ProcessLauncher processLauncher;
    processLauncher.SetProcessData(processes);

    return 0;
}