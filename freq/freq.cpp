#include "freq.hpp"

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <string>

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>

namespace {

bool Comparator(const TDataItem& lhs, const TDataItem& rhs) {
    return lhs.Count > rhs.Count;
}

template <typename T>
void LogErrorItem(std::ostream& msg, T item) {
    msg << item;
}


template <typename T, typename... Args>
void LogErrorItem(std::ostream& msg, T item, Args... args) // recursive variadic function
{
    LogErrorItem(msg, item);
    LogErrorItem(msg, args...);
}

template <typename... Args>
std::string MakeErrorMessage(Args... args) {
    std::ostringstream oss;
    LogErrorItem(oss, args...);
    return oss.str();
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

TFreq::TFreq(std::string inputFilename) : Root(new TNode(nullptr)) {
    ReadData(std::move(inputFilename));
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
        std::runtime_error(MakeErrorMessage("Couldn't open output file: ", outputFilename));
    }

    for (auto& item : Data) {
        outputFile << item.Count << "\t" << item.Word << std::endl;
    }
}

void TFreq::ReadData(std::string inputFilename) {
    // Получаем инфу о ресурсах системы (хотим понять, сколько места можем занять в оперативе)
    struct sysinfo info;
    if (sysinfo(&info) < 0) {
        throw std::runtime_error(MakeErrorMessage("Couldn't receive system info: ", std::strerror(errno)));
    }

    // Открываем файл на чтение
    auto fd = open(inputFilename.c_str(), O_RDONLY);
    if (fd == -1) {
        std::runtime_error(MakeErrorMessage("Couldn't open input file (", inputFilename, "): ", std::strerror(errno)));
    }

    // Получаем инфу о файле
    struct stat fs;
    if (fstat(fd, &fs) == -1) {
        close(fd);
        std::runtime_error(
            MakeErrorMessage("Couldn't receive info about input file (", inputFilename, "): ", std::strerror(errno)));
    }

    // Определяем размер порции для чтение как минимальное число между половиной доступного нам места и размером файла
    size_t bufferSize
        = (static_cast<size_t>(fs.st_size) < info.freeram) ? static_cast<size_t>(fs.st_size) : info.freeram / 2;

    TNode* current = Root;
    std::vector<char> buffer;
    buffer.reserve(bufferSize);
    ssize_t readBytes = 0;
    int index = 0;
    while (1) {
        readBytes = read(fd, buffer.data(), buffer.capacity());
        if (readBytes <= 0) {
            break;
        }
        for (size_t i = 0; i < static_cast<size_t>(readBytes); ++i) {
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

        buffer.clear();
    }

    close(fd);
}

void TFreq::MakeDataVector() {
    std::string word;
    TNode* current = Root;

    while (current) {
        if (current->Count > 0) {
            Data.emplace_back(word, current->Count);
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
