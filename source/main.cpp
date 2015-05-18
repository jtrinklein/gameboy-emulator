#include "gameboy.h"
#include <string>
#include <iostream>

int main(int argc, char**argv) {
    bool skipBios = true;
    std::string skip("--run-bios");

    for (int i = 0; i < argc; ++i) {
        std::cout << i << " - " << argv[i] << std::endl;
        if(std::string(argv[i]) ==  skip) {
            skipBios = false;
        }
    }
    go(skipBios);
	return 0;
}
