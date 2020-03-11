#include "freq.hpp"

#include <cctype>

int main(int argc, char* argv[]) {
    try {
        if (argc < 3) {
            std::cerr << "Incorrect number of arguments (" << argc << ")" << std::endl;
            std::cerr << "Usage: ./freq in.txt out.txt" << std::endl;
            return 1;
        }
        TFreq freq(argv[1]);
        freq.Analyze();
        freq.SaveData(argv[2]);
    } catch (const std::exception& ex) {
        std::cerr << "[Exception] " << ex.what() << std::endl;
    }
    return 0;
}
