#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>

using namespace std;

enum Mode {
    TRUNCATE,
    APPEND
};

class Process {      
    public: 
        Process() {
            cout << "default constructor" << endl;
        }

        Process(string name, string executablePath, string fileStdoutConfig,
                Mode modeStdoutConfig, vector<map<string, string>> cmdArguments) {
            this->name = name;
            this->executablePath = executablePath;
            this->fileStdoutConfig = fileStdoutConfig;
            this->modeStdoutConfig = modeStdoutConfig;
            this->cmdArguments = cmdArguments;
        }

        void start();

    private:
        string name;
        string executablePath;
        string fileStdoutConfig;                   
        Mode modeStdoutConfig;
        vector<map<string, string>> cmdArguments;

};

void Process::start() {
    cout << "system_call" << endl;
    int status = system("./a");

    if (status < 0)
        std::cout << "Error: " << strerror(errno) << '\n';
    else
    {
        if (WIFEXITED(status))
            std::cout << "Program returned normally, exit code " << WEXITSTATUS(status) << '\n';
        else
            std::cout << "Program exited abnormaly\n";
    }
}


int main() {
    vector <Process *> processes;

    processes.push_back(new Process());

    for (auto const process: processes) {
        process->start();
    }

    return 0;
}