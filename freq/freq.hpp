#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>

constexpr size_t ALPHABET_SIZE = 26;

struct TNode {
    TNode(TNode* parent);
    ~TNode();

    size_t Count;
    size_t Index;
    TNode* Parent;
    TNode* Childs[ALPHABET_SIZE];

private:
    TNode(const TNode&);
    TNode& operator=(const TNode&);
};

struct TDataItem {
    std::string Word;
    size_t Count;
    TDataItem(std::string& word, size_t count) : Word(word), Count(count) {}
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
