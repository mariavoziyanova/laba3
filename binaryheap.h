#pragma once

#include <bits/stdc++.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include "common.hpp"

using namespace std;


void swap(int *x, int *y);

// Бинарная куча: реализация через массив
template <class T>
class MinHeap {
  T *h;          // Элементы кучи
  int capacity;  // Размер кучи
  int size;      // Количество элементов
 public:
  // == Конструктор ==
  explicit MinHeap(int capacity) : capacity(capacity) {
    size = 0;
    h = new T[capacity];
  }
  // Количество элементов в бинарной куче
  int getSize() {
    return size;
  }
  // Поддержка основного свойства кучи
  void heapify(int i) {
    int l = left(i);
    int r = right(i);
    int minIdx = i;  // Индекс минимального
    if (l < size && h[l] < h[i]) minIdx = l;
    if (r < size && h[r] < h[minIdx]) minIdx = r;
    if (minIdx != i) {
      swap(h[i], h[minIdx]);
      heapify(minIdx);
    }
  }
  // Индекс родителя
  inline int parent(int i) {
    return (i - 1) / 2;
  }
  // Индекс левого узла
  inline int left(int i) {
    return 2 * i + 1;
  }
  // Индекс правого узла
  inline int right(int i) {
    return 2 * i + 2;
  }
  // Извлечение корня - минимального элемента, который хранится в корне кучи
  int extractMin() {
    if (size <= 0) return INT_MAX;
    if (size == 1) {
      size--;
      return h[0];
    }

    int root = h[0];
    h[0] = h[size - 1];
    size--;
    heapify(0);
    return root;
  }

  void decreaseKey(int i, int new_val) {
    h[i] = new_val;
    while (i != 0 && h[parent(i)] > h[i]) {
      swap(h[i], h[parent(i)]);
      i = parent(i);
    }
  }
  // Минимальное значение - корень кучи
  T getMin() {
    return h[0];
  }

  void deleteKey(int i) {
    decreaseKey(i, INT_MIN);
    extractMin();
  }
  // Добавить новое значение
  void insert(int k) {
    if (size == capacity) throw std::out_of_range("Overflow: Could not insert");
    // Добавляем новое значение в конец кучи
    size++;
    int i = size - 1;
    h[i] = k;
    // Восстанавливаем основное свойство кучи
    while (i != 0 && h[parent(i)] > h[i]) {
      swap(h[i], h[parent(i)]);
      i = parent(i);
    }
  }
  // Поиск по значению
  bool find(T value) {
    return search(value) != -1;
  }
  int search(T value) {
    for (int i = 0; i < size; i++) {
      if (h[i] == value) return i;
    }
    return -1;
  }
  T operator[](size_t index) const {  // Получение значения
    if (index < 0) throw out_of_range("index < 0");
    if (index >= size) throw out_of_range("index >= size");
    return h[index];
  }
  T &operator[](size_t index) {
    if (index < 0) throw out_of_range("index < 0");
    if (index >= size) throw out_of_range("index >= size");
    return h[index];
  }
};
