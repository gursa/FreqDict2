# freq

Программа для подсчета кол-ва вхождений слов в текст.

## Условие

Словом считается набор латинских букв, a-z A-Z. Любой другой символ считается пробелом. Регистр нужно привести. Сортировать нужно сначала по частоте, потом по алфавиту.

## Пример

```
$ cat in.txt
The time has come, the Walrus said,
to talk of many things...
$ freq in.txt out.txt
$ cat out.txt
    2 the
    1 come
    1 has
    1 many
    1 of
    1 said
    1 talk
    1 things
    1 time
    1 to
    1 walrus
```

## Алгоритм

Алогритм базируется на префиксном дереве. Узлом является структура, имеющая до 26 потомков (по кол-ву букв в английском алфавите):
```cpp
const size_t ALPHABET_SIZE = 26;

struct TNode {
    TNode(TNode* parent);
    ~TNode();

    size_t Count;
    size_t Index;
    TNode* Parent;
    TNode* Childs[ALPHABET_SIZE];

private:
    TNode(const TNode& other);
    TNode& operator=(const TNode& other);
};

struct TDataItem {
    std::string Word;
    size_t Count;
};
```
Таким образом, высота дерева не превышает длины самого длинного слова в тексте

## Сборка

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

### Важно

Для сборки необходимо использовать компилятор C++ поддерживающий стандарт C++98.