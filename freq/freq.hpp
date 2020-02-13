#pragma once

#include <fstream>
#include <vector>


class TFreq {
    struct TNode {
        std::string Word;
        unsigned int Count;
        TNode* Left;
        TNode* Right;
        TNode(const std::string& word, unsigned int count = 0);
    };

    struct TDataItem {
        size_t Count;
        std::string Data;
        bool operator<(const TDataItem& rhs);
    };

public:
    TFreq(const std::string& inputFilename);
    virtual ~TFreq();

    void Analyze();
    void SaveData(const std::string& outputFilename);

private:
    TFreq(const TFreq&);
    const TFreq& operator=(const TFreq&);

    void ReadData(std::ifstream& inputFile);
    void AddWord(const std::string& word);
    void DeleteNode(TNode* node);
    void NodeToDataVector(TNode* node);

private:
    TNode* Root;
    std::vector<TDataItem> Data;
};
