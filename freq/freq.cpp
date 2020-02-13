#include "freq.hpp"

#include <algorithm>

TFreq::TFreq(const std::string& inputFilename) {
    Root = new TNode("");
    std::ifstream inputFile(inputFilename.c_str());
    if (!inputFile.is_open()) {
        std::runtime_error("Couldn't open input file" + inputFilename);
    }

    ReadData(inputFile);
}

TFreq::~TFreq() {
    DeleteNode(Root);
}

void TFreq::AddWord(const std::string& word) {
    TNode* node = Root;
    while (node) {
        int result = node->Word.compare(word);
        if (!result) {
            node->Count++;
            break;
        } else if (result < 0) {
            if (node->Left) {
                node = node->Left;
                continue;
            } else {
                node->Left = new TNode(word, 1);
                break;
            }
        } else {
            if (node->Right) {
                node = node->Right;
                continue;
            } else {
                node->Right = new TNode(word, 1);
                break;
            }
        }
    }
}

void TFreq::DeleteNode(TNode* node) {
    if (node == NULL) {
        return;
    }

    if (node->Left) {
        DeleteNode(node->Left);
    }

    if (node->Right) {
        DeleteNode(node->Right);
    }

    delete node;
}

void TFreq::Analyze() {
    if (Root->Left) {
        NodeToDataVector(Root->Left);
    }

    if (Root->Right) {
        NodeToDataVector(Root->Right);
    }
    std::sort(Data.begin(), Data.end());
}

void TFreq::SaveData(const std::string& outputFilename) {
    std::ofstream outputFile(outputFilename.c_str());
    if (!outputFile.is_open()) {
        std::runtime_error("Couldn't open output file" + outputFilename);
    }

    for (std::vector<TDataItem>::iterator it = Data.begin(); it < Data.end(); ++it) {
        outputFile << it->Data << "\t" << it->Count << std::endl;
    }
}

void TFreq::ReadData(std::ifstream& inputFile) {
    std::string line;
    char c;
    std::string word;
    while (std::getline(inputFile, line)) {
        for (size_t i = 0; i < line.length(); i++) {
            c = line.at(i);
            if (std::isalpha(c, std::locale::classic())) {
                word.append(1, std::tolower(c, std::locale::classic()));
            } else {
                if (word.size()) {
                    AddWord(word);
                    word.clear();
                }
            }
        }
    }
}

void TFreq::NodeToDataVector(TNode* node) {
    if (node == NULL) {
        return;
    }

    TDataItem item = {node->Count, node->Word};
    Data.push_back(item);

    if (node->Left) {
        NodeToDataVector(node->Left);
    }

    if (node->Right) {
        NodeToDataVector(node->Right);
    }
}

bool TFreq::TDataItem::operator<(const TFreq::TDataItem& rhs) {
    if (Count > rhs.Count) {
        return true;
    }

    if (Count < rhs.Count) {
        return false;
    }

    if (Data < rhs.Data) {
        return true;
    }

    return false;
}

TFreq::TNode::TNode(const std::string& word, unsigned int count) : Word(word), Count(count) {
}
