#pragma once

// == АТД (абстрактные типы данных) ==

#include <algorithm>
#include <string>

// using namespace std;  // Чтобы не писать перед каждым классом Standard Template Library (STL) std::

// _countof определён в Visual C++ - возвращает количество элементов в массиве
// Его нет в GNU C++ - приходится определять макрос чтобы он работал одинаково
#ifndef _countof
#define _countof(a) (sizeof(a) / sizeof(*(a)))
#endif

// == Исключения ==

// Выход индекса за допустимые границы: отрицательный, >= размера массива
// Индекс должен быть в пределах: 0..количество_элементов-1
class IndexOutOfRange : public std::exception {  // наследуем наш класс-исключение от общего
  std::string message_;                          // Детали об ошибке в читаемом формате

 public:
  explicit IndexOutOfRange(const std::string &message) : message_(message) {}
  // Чтобы не писать IndexOutOfRange(string("сообщение об ошибке")) в простейших случаях
  // Можно использовать: IndexOutOfRange("сообщение об ошибке")
  explicit IndexOutOfRange(const char *message) : message_(message) {}
  // Метод, который возвращает читаемое сообщение об ошибке
  const char *what() const noexcept override {
    return message_.c_str();
  }
};

// trim from start (in place)
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
    return !std::isspace(ch);
  }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
    return !std::isspace(ch);
  }).base(), s.end());
}

static inline std::string trim_copy(std::string s) {
  ltrim(s);
  rtrim(s);
  return s;
}
