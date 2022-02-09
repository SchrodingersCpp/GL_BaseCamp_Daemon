#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "b) There are " << argc << " arguments:\n";

    for (int count{ 0 }; count < argc; ++count) {
        std::cout << count << ' ' << argv[count] << '\n';
    }

    return 0;
}