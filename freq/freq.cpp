#include "freq.hpp"

#include <algorithm>
#include <cstring>
#include <string>

namespace {

bool Comparator(const TDataItem &lhs, const TDataItem &rhs) {
  return lhs.Count > rhs.Count;
}

} // namespace

TNode::TNode(TNode *parent) : Count(0), Index(0), Parent(parent) {
  memset(Childs, 0, sizeof(Childs));
}

TNode::~TNode() {
  for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
    delete Childs[i];
  }
}

TFreq::TFreq(const std::string &inputFilename) {
  Root = new TNode(0);
  std::ifstream inputFile(inputFilename.c_str());
  if (!inputFile.is_open()) {
    std::runtime_error("Couldn't open input file" + inputFilename);
  }

  ReadData(inputFile);
}

TFreq::~TFreq() { delete Root; }

void TFreq::Analyze() {
  MakeDataVector();
  std::stable_sort(Data.begin(), Data.end(), Comparator);
}

void TFreq::SaveData(const std::string &outputFilename) {
  std::ofstream outputFile(outputFilename.c_str());
  if (!outputFile.is_open()) {
    std::runtime_error("Couldn't open output file" + outputFilename);
  }

  for (std::vector<TDataItem>::iterator it = Data.begin(); it < Data.end();
       ++it) {
    outputFile << it->Word << "\t" << it->Count << std::endl;
  }
}

void TFreq::ReadData(std::ifstream &inputFile) {
  TNode *current = Root;
  std::string line;
  char symbol;
  while (std::getline(inputFile, line)) {
    for (size_t i = 0; i < line.length(); ++i) {
      symbol = line[i];
      if (std::isalpha(symbol, std::locale::classic())) {
        int index = std::tolower(symbol, std::locale::classic()) - 'a';

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

    if (current != Root) {
      ++current->Count;
    }
  }
}

void TFreq::MakeDataVector() {
  std::string word;
  TNode *current = Root;

  while (current) {
    if (current->Count > 0) {
      Data.resize(Data.size() + 1);
      Data.back().Word = word;
      Data.back().Count = current->Count;
    }
    bool foundChild = false;
    for (size_t i = current->Index; i < ALPHABET_SIZE && !foundChild; ++i) {
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
