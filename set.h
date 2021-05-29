#pragma once

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>

#include "binarytree.h"
#include "common.hpp"

using namespace std;

// Множество
template <typename T>
class Set {
  BinaryTree<T> tree;  // Для реализации используется бинарное дерево поиска
 public:
  // == Конструкторы - инициализация ==
  Set() = default;  // Пустое множество
  // В STL е
  // Инициализация из std::set
  Set(std::set<T> set) : tree(set) {}
  // И из списка значений при инициализации, например: Set<int> s {1,3}
  Set(initializer_list<T> list) : tree(list) {}
  // Инициализация из строки
  explicit Set(const char *str) {
    istringstream in(str);
    T value;  // Очередное значение из строки
    while (in >> value) tree.insert(value);
  }
  explicit Set(const string &str) {
    istringstream in(str);
    T value;
    while (in >> value) tree.insert(value);
  }
  // map, reduce, where
  // map - применение функции к каждому элементу множества
  Set<T> map(T f(T)) {
    Set<T> res;  // Создаётся новое множество
    for (T x : tree) {
      res.insert(f(x));
    }
    return res;
  }
  // where фильтрует значения из списка l с помощью функции-фильтра h
  Set<T> *where(bool h(T)) {
    auto *res = new Set<T>;
    for (T x : tree)
      if (h(x)) {
        res->insert(x);
      }
    return res;
  }
  // reduce - применяем к каждой паре значений пока не получим одно значение
  T reduce(T f(T, T)) {
    return tree.reduce(f);
  }
  // Размер множества
  int size() const {
    return tree.getSize();
  }
  // Добавить значение в множество
  void insert(const T &value) {
    if (tree.find(value)) return;  // Если такое значение уже есть => не добавляем
    tree.insert(value);            // Если нет значения, то добавляем
  }
  // Поиск значения в множестве
  bool find(const T &value) const {
    return tree.find(value);
  }
  // Удаление значения из множества
  void erase(const T &value) {
    tree.remove(value);
  }
  // Объединение множеств
  Set<T> setUnion(Set<T> &s) {
    Set<T> res;  // Итоговое множество
    for (T x : tree) res.insert(x);  // Берём "наше" дерево поиска и добавляем все элементы из него
    for (T x : s) res.insert(x);  // Берём второе множество и добавляем все элементы из него
    // За счёт того что повторно элементы не добавляются
    return res;
  }
  // Пересечение множеств
  Set<T> intersection(Set<T> &s) {
    Set<T> res;
    for (T x : tree)  // Пробегаем по всем элементам нашего множества
      if (s.find(x))  // Если элемент содержится и в другом множестве
        res.insert(x);
    return res;
  }
  // Вычитание множеств: в результат войдут все "наши" элементы которых нет во втором множестве
  Set<T> difference(Set<T> &s) {
    Set<T> res;
    for (T x : tree)   // Пробегаем по всем элементам нашего множества
      if (!s.find(x))  // Если элемент не содержится в другом множестве
        res.insert(x);
    return res;
  }
  // Является ли текущее множество подмножеством другого?
  bool subSet(const Set<T> &set) const {
    for (T x : tree) {  // Перебираем все элементы нашего множества
      if (!set.find(x)) return false;  // Если какой-то элемент не найден => не является подмножеством
    }
    return true;  // Если все найдены, то является подмножеством
  }
  // Проверка на равенство (двух множеств): равны ли множества?
  bool equal(const Set<T> &set) const {
    return this->subSet(set) && set.subSet(*this);
  }
  // Сохраним в строку
  string toString() const {
    // Сохраним элементы как вектор
    vector<T> elements;
    for (T x : tree) elements.push_back(x);
    // Отсортируем вектор по неубыванию
    sort(elements.begin(), elements.end());
    // Напечатаем отсортированные элементы в строку
    stringstream ss;
    for (T x : elements) ss << x << " ";
    return trim_copy(ss.str());
  }

  struct Iterator {
    using iterator_category = forward_iterator_tag;
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    typename BinaryTree<T>::Iterator iterator;
    // Используем итератор для вложенной структуры
    explicit Iterator(typename BinaryTree<T>::Iterator iterator) : iterator(iterator) {}
    reference operator*() const {
      return *iterator;
    }
    pointer operator->() {
      return iterator;
    }
    Iterator &operator++() {
      ++iterator;
      return *this;
    }
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    friend bool operator==(const Iterator &a, const Iterator &b) {
      return a.iterator == b.iterator;
    };
    friend bool operator!=(const Iterator &a, const Iterator &b) {
      return a.iterator != b.iterator;
    };
  };
  Iterator begin() const {
    return Iterator(tree.begin());
  }
  Iterator end() const {
    return Iterator(tree.end());
  }
};
