#pragma once

#include <cstddef>
#include <fstream>
#include <vector>

constexpr std::size_t ALPHABET_SIZE = 26;

struct TNode {
    TNode(TNode* parent);
    ~TNode();

    std::size_t Count;
    std::size_t Index;
    TNode* Parent;
    TNode* Childs[ALPHABET_SIZE];

private:
    TNode(const TNode&);
    TNode& operator=(const TNode&);
};

struct TDataItem {
    std::string Word;
    std::size_t Count;
};

class TFreq {
public:
    explicit TFreq(std::string inputFilename);
    virtual ~TFreq();

    void Analyze();
    void SaveData(const std::string& outputFilename);

private:
    TFreq(const TFreq&) = delete;
    const TFreq& operator=(const TFreq&) = delete;

    void ReadData(std::string inputFilename);
    void MakeDataVector();

private:
    TNode* Root;
    std::vector<TDataItem> Data;
};
