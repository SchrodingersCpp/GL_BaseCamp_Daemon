#include <string>
#include <fstream>
#include <cassert>
#include <vector>
#include <map>

using std::string;
using std::vector;
using vecstr = vector<string>;
using dict = std::map<string, string>;

class Process
{
private:
    string name;
    string exePath;
    string stdoutConfigFile;
    string stdoutConfigMode;
    vecstr cmdArgs;

public:
    Process() : name{""}, exePath{""}, stdoutConfigFile{""},
                stdoutConfigMode{"truncate"}, cmdArgs{}
    {
    }

    // setters
    void setName(string iname) { name = iname; }
    void setExePath(string ipath) { exePath = ipath; }
    void setStdoutConfigFile(string ifile) { stdoutConfigFile = ifile; }
    void setStdoutConfigMode(string imode) { stdoutConfigMode = imode; }
    void addCmdArgs(string iarg) { cmdArgs.push_back(iarg); }

    // getters
    string getName() { return name; }
    string getExePath() { return exePath; }
    string getStdoutConfigFile() { return stdoutConfigFile; }
    string getStdoutConfigMode() { return stdoutConfigMode; }
    vecstr getCmdArgs() { return cmdArgs; }
};

void fillProcessObject(dict &flags, string &flag, string &flagValue,
                       Process &proc)
{
    if (flag == flags["flagName"])
    {
        proc.setName(flagValue);
        return;
    }
    else if (flag == flags["flagExePath"])
    {
        proc.setExePath(flagValue);
        return;
    }
    else if (flag == flags["flagFile"])
    {
        proc.setStdoutConfigFile(flagValue);
        return;
    }
    else if (flag == flags["flagMode"])
    {
        proc.setStdoutConfigMode(flagValue);
        return;
    }
    else if (flag == flags["flagOptionName"])
    {
        proc.addCmdArgs(flagValue);
        return;
    }
}

void getFlagValue(string &flag, string &line, string &flagValue)
{
    // skip flag portion from line string
    const auto i{line.find(flag) + flag.length()};
    string value{line.substr(i)};

    // trim leading and trailing whitespaces (if any)
    const char whitespace[]{" \t"};
    const auto iBegin{value.find_first_not_of(whitespace)};
    const auto iFinal{value.find_last_not_of(whitespace)};
    flagValue = value.substr(iBegin, iFinal - iBegin + 1);
}

void createProcessObjects(vecstr &yamlContent, dict &flags,
                          vector<Process> &procs)
{
    // delete unnecessary flags
    flags.erase("flagStdoutConfig");
    flags.erase("flagCmdArgs");

    for (auto &line : yamlContent)
    {
        for (auto &flag : flags)
        {

            // check if line contains flag
            const auto flagFound{line.find(flag.second)};

            if (flagFound != string::npos)
            {

                // create process object at each "Name" flag
                if (flag.first == "flagName")
                {
                    procs.push_back(Process());
                }

                string flagValue{};
                getFlagValue(flag.second, line, flagValue);
                fillProcessObject(flags, flag.second, flagValue, procs.back());
                break;
            }
        }
    }
}

// check if YAML-file content is valid
void yamlContentValid(vecstr &yamlContent, dict &flags)
{

    // check if YAML-file content starts with "processes" flag
    assert((yamlContent[0].find(flags["flagProcesses"]) != string::npos) && "YAML should start with 'processes:' key!");

    // delete "processes" flag line (not needed for further processing)
    yamlContent.erase(yamlContent.begin());
    flags.erase("flagProcesses");

    // check if rest of YAML-file content has valid flags
    for (auto &line : yamlContent)
    {
        bool validFlag{false};
        for (auto &flag : flags)
        {
            if (line.find(flag.second) != string::npos)
            {
                validFlag = true;
                break;
            }
        }
        assert(validFlag == true && "YAML contains invalid flag!");
    }
}

void readYAML(vecstr &yamlContent)
{
    // open YAML-file
    string yamlFileName{"./config.yaml"};
    std::fstream yamlFile{};
    yamlFile.open(yamlFileName, std::fstream::in);

    // check if YAML-file exists
    assert((yamlFile.is_open() == true) && "File not found!");

    // read YAML-file content
    string yamlLine{};
    while (std::getline(yamlFile, yamlLine))
    {
        if (yamlLine != "")
        { // exclude empty lines
            yamlContent.push_back(yamlLine);
        }
    }

    // close YAML-file
    yamlFile.close();
}

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








int main()
{
    vecstr yamlContent{};
    readYAML(yamlContent);

    dict yamlFlags{{"flagProcesses", "processes:"},
                   {"flagName", " name:"},
                   {"flagExePath", " executable-path:"},
                   {"flagStdoutConfig", " stdout-config:"},
                   {"flagFile", " file:"},
                   {"flagMode", " mode:"},
                   {"flagCmdArgs", " cmd-arguments:"},
                   {"flagOptionName", " option-name:"}};
    yamlContentValid(yamlContent, yamlFlags);

    vector<Process> procs{};
    createProcessObjects(yamlContent, yamlFlags, procs);

    ProcessLauncher processLauncher;

    processLauncher.SetProcessData(procs);

    return 0;
}