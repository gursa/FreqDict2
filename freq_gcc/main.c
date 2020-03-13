#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define READ_BUFFER_SIZE 100*1024*1024

struct TTrieNode {
    size_t Count;
    size_t Index;
    struct TTrieNode* Parent;
    struct TTrieNode* Childs[ALPHABET_SIZE];
};

struct TDataItem {
    char* Word;
    size_t Count;
    struct TDataItem* Next;
};

struct TTrieNode* TrieCreateNode(struct TTrieNode* parent);
void TrieClear(struct TTrieNode** trieRef);
int TrieLoadFromFile(const char* fileName, struct TTrieNode** trieRef);
void GetWordFromTrie(struct TDataItem** listRef, struct TTrieNode* trie, char word[], int level);
int MakeDataList(struct TDataItem** listRef, struct TTrieNode* trie);

void ListPush(struct TDataItem** listRef, const char* word, const size_t count);
void ListFree(struct TDataItem** listRef);
void ListFrontBackSplit(struct TDataItem* source, struct TDataItem** frontRef, struct TDataItem** backRef);
struct TDataItem* ListComparator(struct TDataItem* left, struct TDataItem* right);
int ListMergeSort(struct TDataItem** listRef);
int ListSaveData(const char* fileName, struct TDataItem* item);
int ClearAndExit(struct TTrieNode** trieRef, struct TDataItem** listRef, const int exitCode);

int main(int argc, char* argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Incorrect number of arguments (%d)\n", argc);
        fprintf(stderr, "Usage: ./freq in.txt out.txt\n");
        return EXIT_FAILURE;
    }
    struct TTrieNode* trie = TrieCreateNode(NULL);
    struct TDataItem* list = NULL;

    if(TrieLoadFromFile(argv[1], &trie)) {
        return ClearAndExit(&trie, &list, EXIT_FAILURE);
    }

    if(MakeDataList(&list, trie)) {
        return ClearAndExit(&trie, &list, EXIT_FAILURE);
    }

    if(ListMergeSort(&list)) {
        return ClearAndExit(&trie, &list, EXIT_FAILURE);
    }

    if(ListSaveData(argv[2], list)) {
        return ClearAndExit(&trie, &list, EXIT_FAILURE);
    }


    return EXIT_SUCCESS;
}

struct TTrieNode* TrieCreateNode(struct TTrieNode* parent){
    struct TTrieNode* node = NULL;
    node = (struct TTrieNode*)malloc(sizeof (struct TTrieNode));
    if(node != NULL) {
        node->Count = 0;
        node->Index = 0;
        node->Parent = parent;
        for(int i = 0; i < ALPHABET_SIZE; ++i) {
            node->Childs[i] = NULL;
        }
    } else {
        fprintf(stderr, "[%s] Failed to allocate memory for trie node\n", __func__);
    }
    return node;
}

void TrieClear(struct TTrieNode** trieRef) {
    if(*trieRef == NULL) {
        return;
    }
    for(int i = 0; i < ALPHABET_SIZE; ++i) {
        TrieClear(&((*trieRef)->Childs[i]));
    }
    free(*trieRef);
}

int TrieLoadFromFile(const char* fileName, struct TTrieNode** trieRef) {
    if(fileName == NULL) {
        fprintf(stderr, "[%s] Invalid file name\n", __func__);
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(fileName, "rb");
    if(fp == NULL) {
        fprintf(stderr, "[%s] Could not open file %s:\t%s\n", __func__, fileName, strerror(errno));
        return EXIT_FAILURE;
    }
    char* buffer = (char*)malloc(READ_BUFFER_SIZE * sizeof(char));
    if(buffer == NULL) {
        fprintf(stderr, "[%s] Failed to allocate memory for read buffer\n", __func__);
        fclose(fp);
        return EXIT_FAILURE;
    }
    size_t readBytes = 0;
    int index = 0;
    struct TTrieNode* current = *trieRef;
    while (1) {
        readBytes = fread(buffer, sizeof (char), READ_BUFFER_SIZE, fp);
        if(!readBytes) {
            break;
        }
        for(size_t i = 0; i < readBytes; ++i){
            if ((buffer[i] >= 'A') && (buffer[i] <= 'Z')) {
                buffer[i] |= 0x20; // приводим к нижнему регистру
            }
            if ((buffer[i] >= 'a') && (buffer[i] <= 'z')) {
                index = buffer[i] - 'a';

                if (!current->Childs[index]) {
                    current->Childs[index] = TrieCreateNode(current);
                }
                current = current->Childs[index];
            } else {
                if (current != *trieRef) {
                    ++current->Count;
                    current = *trieRef;
                }
            }
        }
        memset(buffer, 0, READ_BUFFER_SIZE);
    }
    free(buffer);
    fclose(fp);
    return EXIT_SUCCESS;
}

void GetWordFromTrie(struct TDataItem** listRef, struct TTrieNode* trie, char word[], int level) {
    if(trie == NULL) {
        return;
    } else {
        if(trie->Count) {
            word[level] = '\0';
            ListPush(listRef, word, trie->Count);
        }
    }
    for(size_t i = trie->Index; i < ALPHABET_SIZE; ++i) {
        if(trie->Childs[i]) {
            word[level] = (char)i + 'a';
            GetWordFromTrie(listRef, trie->Childs[i], word, level+1);
        }
    }
}

int MakeDataList(struct TDataItem** listRef, struct TTrieNode* trie) {
    if(trie == NULL) {
        fprintf(stderr, "[%s] Invalid trie node\n", __func__);
        return EXIT_FAILURE;
    }
    char word[256];
    int level = 0;
    GetWordFromTrie(listRef, trie, word, level);
    return EXIT_SUCCESS;
}

void ListPush(struct TDataItem** listRef, const char* word, const size_t count) {
    struct TDataItem* item = (struct TDataItem*)malloc(sizeof(struct TDataItem));
    if(item == NULL) {
        fprintf(stderr, "[%s] Failed to allocate memory for list node: {%s;%zu}\n", __func__, word, count);
        return;
    }

    item->Word = strdup(word);
    item->Count = count;
    item->Next = (*listRef);
    (*listRef) = item;
}

void ListFree(struct TDataItem **listRef) {
    struct TDataItem* item = NULL;
    while(*listRef != NULL) {
        item = *listRef;
        *listRef = (*listRef)->Next;
        free(item);
    }
}

void ListFrontBackSplit(struct TDataItem* source, struct TDataItem** frontRef, struct TDataItem** backRef) {
    struct TDataItem* fast;
    struct TDataItem* slow;
    slow = source;
    fast = source->Next;

    while (fast != NULL) {
        fast = fast->Next;
        if (fast != NULL) {
            slow = slow->Next;
            fast = fast->Next;
        }
    }

    *frontRef = source;
    *backRef = slow->Next;
    slow->Next = NULL;
}

struct TDataItem* ListComparator(struct TDataItem* left, struct TDataItem* right) {
    struct TDataItem* result = NULL;

    if (left == NULL)
        return (right);
    else if (right == NULL)
        return (left);

    if (left->Count > right->Count) {
        result = left;
        result->Next = ListComparator(left->Next, right);
        return result;
    }
    if (left->Count < right->Count) {
        result = right;
        result->Next = ListComparator(left, right->Next);
        return result;
    }

    int compareResult = strcmp(left->Word, right->Word);
    if(compareResult > 0) {
        result = right;
        result->Next = ListComparator(left, right->Next);
        return result;
    } else {
        result = left;
        result->Next = ListComparator(left->Next, right);
        return result;
    }
}

int ListMergeSort(struct TDataItem** listRef) {
    struct TDataItem* head = *listRef;
    struct TDataItem* front;
    struct TDataItem* back;

    if ((head == NULL) || (head->Next == NULL)) {
        return EXIT_FAILURE;
    }

    ListFrontBackSplit(head, &front, &back);
    ListMergeSort(&front);
    ListMergeSort(&back);

    *listRef = ListComparator(front, back);
    return EXIT_SUCCESS;
}

int ListSaveData(const char* fileName, struct TDataItem* item) {
    if(fileName == NULL) {
        fprintf(stderr, "[%s] Invalid file name\n", __func__);
        return EXIT_FAILURE;
    }
    FILE *fp = fopen(fileName, "w");
    if(fp == NULL) {
        fprintf(stderr, "[%s] Could not open file %s:\t%s\n", __func__, fileName, strerror(errno));
        return EXIT_FAILURE;
    }
    while (item != NULL) {
        if(fprintf(fp, "%zu\t%s\n", item->Count, item->Word) < 0) {
            fprintf(stderr,
                    "[%s] An error occurred while writing data {%s; %zu} to the file %s: %s\n",
                    __func__,
                    item->Word,
                    item->Count,
                    fileName,
                    strerror(errno));
            fclose(fp);
            return EXIT_FAILURE;
        }
        item = item->Next;
    }
    fclose(fp);
    return EXIT_SUCCESS;
}

int ClearAndExit(struct TTrieNode** trieRef, struct TDataItem** listRef, const int exitCode) {
    ListFree(listRef);
    TrieClear(trieRef);
    return exitCode;
}
