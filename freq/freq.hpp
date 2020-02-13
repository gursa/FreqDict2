#pragma once

#include <cstddef>
#include <fstream>
#include <vector>

const size_t ALPHABET_SIZE = 26;

struct TNode {
  TNode(TNode *parent);
  ~TNode();

  size_t Count;
  size_t Index;
  TNode *Parent;
  TNode *Childs[ALPHABET_SIZE];

private:
  TNode(const TNode &other);
  TNode &operator=(const TNode &other);
};

struct TDataItem {
  std::string Word;
  size_t Count;
};

class TFreq {
public:
  TFreq(const std::string &inputFilename);
  virtual ~TFreq();

  void Analyze();
  void SaveData(const std::string &outputFilename);

private:
  TFreq(const TFreq &);
  const TFreq &operator=(const TFreq &);

  void ReadData(std::ifstream &inputFile);
  void MakeDataVector();

private:
  TNode *Root;
  std::vector<TDataItem> Data;
};
