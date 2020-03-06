#include "freq.hpp"

#include <algorithm>
#include <cstring>
#include <string>
#include <cstdio>

namespace {

bool Comparator(const TDataItem& lhs, const TDataItem& rhs) {
    return lhs.Count > rhs.Count;
}

} // namespace

TNode::TNode(TNode* parent) : Count(0), Index(0), Parent(parent) {
    memset(Childs, 0, sizeof(Childs));
}

TNode::~TNode() {
    for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
        delete Childs[i];
    }
}

TFreq::TFreq(const std::string& inputFilename) {
    Root = new TNode(0);
    ReadData(inputFilename);
}

TFreq::~TFreq() {
    delete Root;
}

void TFreq::Analyze() {
    MakeDataVector();
    std::stable_sort(Data.begin(), Data.end(), Comparator);
}

void TFreq::SaveData(const std::string& outputFilename) {
    std::ofstream outputFile(outputFilename.c_str());
    if (!outputFile.is_open()) {
        std::runtime_error("Couldn't open output file" + outputFilename);
    }

    for (std::vector<TDataItem>::iterator it = Data.begin(); it < Data.end(); ++it) {
        outputFile << it->Word << "\t" << it->Count << std::endl;
    }
}

void TFreq::ReadData(const std::string& inputFilename) {
    TNode* current = Root;
    char buffer[4 * 1024];
    size_t readBytes = 0;
    int index = 0;
    FILE* inputFile = fopen(inputFilename.c_str(), "rb");
    if (inputFile == nullptr) {
        std::runtime_error("Couldn't open input file" + inputFilename);
    }
    while ((readBytes = fread(buffer, 1, sizeof(buffer), inputFile)) > 0) {
        for (size_t i = 0; i < readBytes; ++i) {
            if ((buffer[i] >= 'A') && (buffer[i] <= 'Z')) {
                buffer[i] |= 0x20; // приводим к нижнему регистру
            }

            if ((buffer[i] >= 'a') && (buffer[i] <= 'z')) {
                index = buffer[i] - 'a';

                if (!current->Childs[index]) {
                    current->Childs[index] = new TNode(current);
                }
                current = current->Childs[index];
            } else {
                if (current != Root) {
                    ++current->Count;
                    current = Root;
                }
            }
        }

        memset(buffer, 0, sizeof(buffer));
    }
    fclose(inputFile);
}

void TFreq::MakeDataVector() {
    std::string word;
    TNode* current = Root;

    while (current) {
        if (current->Count > 0) {
            Data.resize(Data.size() + 1);
            Data.back().Word = word;
            Data.back().Count = current->Count;
            current->Count = 0;
        }
        bool foundChild = false;
        for (auto i = current->Index; i < ALPHABET_SIZE && !foundChild; ++i) {
            if (current->Childs[i]) {
                word.push_back(static_cast<char>('a' + i));
                current->Index = i + 1;
                current = current->Childs[i];
                foundChild = true;
            }
        }
        if (!foundChild) {
            current = current->Parent;
            if (!word.empty()) {
                word.resize(word.size() - 1);
            }
        }
    }
}
