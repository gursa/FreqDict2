# freq

Программа для подсчета кол-ва вхождений слов в текст.

## Алгоритм

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

## Сборка

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

### Важно

Для сборки необходимо использовать компилятор C++ поддерживающий стандарт C++98.