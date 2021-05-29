// Консольная программа для демонстрации

#include <fcntl.h>

#include <chrono>
#include <complex>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cwchar>

#include "binarytree.h"
#include "common.hpp"
#include "menu.h"
#include "set.h"

using namespace std;

// Функиия, которую можно применить к каждому элементу последовательности
template <class T>
T map_function(T x) {
  return x * x;
}

std::wstring map_function(std::wstring x) {
  return x + L"!";
}

// Фильтры для разных типов данных. Для каждого типа - своя
bool where_function(int x) {
  bool result = (x % 2) == 0;
  std::wcout << L"  where: число " << x << L" чётное: " << result << std::endl;
  return result;
}

bool where_function(double x) {
  bool result = abs(x) < 5;
  std::wcout << L"  where: число |" << x << L"| < 5: " << result << std::endl;
  return result;
}

bool where_function(std::complex<double> x) {
  bool result = abs(x) < 5;
  std::wcout << L"  where: число |" << x << L"| < 5: " << result << std::endl;
  return result;
}

bool where_function(const std::wstring x) {
  bool result = x.size() < 4;
  std::wcout << L"  where: длина строки " << x << L" < 4: " << result << std::endl;
  return result;
}

// Сложение для всех чисел (конкатенация для строк)
template <class T>
T reduce_function(T a, T b) {
  T result = a + b;
  std::wcout << L"  reduce: " << a << L" + " << b << " = " << result << std::endl;
  return result;
}

// Выполнить тестирование скорости работы алгоритмов на больших (10^4-10^5 элементов)
// Замеряем время работы дерева
void treeImplementationSpeed() {
  auto begin = chrono::steady_clock::now();  // Засекаем начало работы

  BinaryTree<int> tree;       // Создаём дерево
  const int numbers = 20000;  // Добавим числа
  wcout << L"Количество элементов для тестирования: " << numbers << endl;
  for (int i = 1; i <= numbers; i++) {
    tree.insert(i);
  }
  auto end = chrono::steady_clock::now();  // Конец работы
  auto elapsed_mcs = chrono::duration_cast<chrono::microseconds>(end - begin);
  // Вычисляем разницу в секундах времени начала и окончания работы
  const double t = elapsed_mcs.count() / 1e6;
  // Выводим результат в секундах на экран (в консоль)
  wcout << typeid(tree).name() << " time = " << t << endl;
}

template <class T>
void stack_addElementSpeed() {
  wprintf(L"Сравнение времени добавления элементов в дерево\n");
  treeImplementationSpeed();
}

#define PRINT(x) wprintf(L"%s = %d\n", #x, x);

template <class T>
void tree_base_operations() {
  BinaryTree<T> bt;
  while (true) {
    bt.printAsTree();
    wcout << L"Операции с деревом:" << endl;
    wcout << L"1. Вставка" << endl;
    wcout << L"2. Удаление" << endl;
    wcout << L"3. Поиск" << endl;
    wcout << L"0. Выход" << endl;
    int choice;
    wcin >> choice;
    T element;
    switch (choice) {
    case 1:
      wcout << L"Вставка: введите элемент для вставки: ";
      wcin >> element;
      bt.insert(element);
      break;
    case 2:
      wcout << L"Удаление: введите элемент для удаления: ";
      wcin >> element;
      bt.remove(element);
      break;
    case 3:
      wcout << L"Поиск: введите элемент для поиска: ";
      wcin >> element;
      if (bt.find(element)) {
        wcout << L"Элемент найден" << endl;
      } else {
        wcout << L"Элемент не найден" << endl;
      }
      break;
    case 0:
      return;
    default:
      wcout << L"Нет такого пункта меню" << endl;
    }
  }
}

template <class T>
void set_base_operations() {
  Set<T> st;
  while (true) {
    st.toString();
    wcout << L"Операции с множеством:" << endl;
    wcout << L"1. Вставка" << endl;
    wcout << L"2. Удаление" << endl;
    wcout << L"3. Поиск" << endl;
    wcout << L"0. Выход" << endl;
    int choice;
    wcin >> choice;
    T element;
    switch (choice) {
      case 1:
        wcout << L"Вставка: введите элемент для вставки: ";
        wcin >> element;
        st.insert(element);
        break;
      case 2:
        wcout << L"Удаление: введите элемент для удаления: ";
        wcin >> element;
        st.erase(element);
        break;
      case 3:
        wcout << L"Поиск: введите элемент для поиска: ";
        wcin >> element;
        if (st.find(element)) {
          wcout << L"Элемент найден" << endl;
        } else {
          wcout << L"Элемент не найден" << endl;
        }
        break;
      case 0:
        return;
      default:
        wcout << L"Нет такого пункта меню" << endl;
    }
  }
}
template <class T>
void apply_map_where_reduce() {
  wprintf(L"Применение функций map, where, reduce - ручной ввод данных\n");
  BinaryTree<T> tree;  // Дерево из элементов типа T
  wcout << L"Введите количество элементов: ";
  int N;
  wcin >> N;
  for (int i = 1; i <= N; i++) {
    T element;
    wcout << L"Введите элемент номер " << i << ": ";
    wcin >> element;
    tree.insert(element);
  }
  tree.printAsTree();

  wprintf(L"Применяем операцию map (построить новое дерево поэлементным преобразованием)\n");
  BinaryTree<T> *mapRes = tree.map(map_function);
  mapRes->printAsTree();  // Печатаем дерево
  delete mapRes;          // Очищаем память

  wprintf(L"Применяем операцию where\n");
  BinaryTree<T> *whereRes = tree.where(where_function);
  whereRes->printAsTree();
  delete whereRes;  // Очищаем память

  wprintf(L"Применяем операцию reduce - сложение для всех чисел (конкатенация для строк)\n");
  T reduceRes = tree.reduce(reduce_function);
  std::wcout << L"Результат reduce: " << reduceRes << std::endl << std::endl;

  wprintf(L"\n");
}

//template <class T>
//void thread() {
//  wprintf(L"Прошивка дерева\n");
//  BinaryTree<T> tree;  // Дерево из элементов типа T
//  wcout << L"Введите количество элементов: ";
//  int N;
//  wcin >> N;
//  for (int i = 1; i <= N; i++) {
//    T element;
//    wcout << L"Введите элемент номер " << i << ": ";
//    wcin >> element;
//    tree.insert(element);
//  }
//  tree.printAsTree();
//  wprintf(L"Прошивка дерева в порядке Корень Левое Правое\n");
//  BinaryTree<T> *thread = tree.threadAsVector();
//  thread->printAsTree();  // Печатаем дерево
//  delete thread;          // Очищаем память
//
//}


template <class T>
void apply_map_where_reduce_set() {
  wprintf(L"Применение функций map, where, reduce - ручной ввод данных\n");
  Set<T> Set;  // Множество из элементов типа T
  wcout << L"Введите количество элементов: ";
  int N;
  wcin >> N;
  for (int i = 1; i <= N; i++) {
    T element;
    wcout << L"Введите элемент номер " << i << ": ";
    wcin >> element;
    Set.insert(element);
  }
  Set.toString();

  wprintf(L"Применяем операцию map (построить новое множество поэлементным преобразованием)\n");
  //Set<T> mapRes = Set.map(map_function);
  //std::wcout << L"Результат reduce: " << mapRes << std::endl << std::endl;
  //wprintf(L"Применяем операцию where\n");
  //Set<T> whereRes = Set.where(where_function);
 // whereRes->printAsTree();
 // delete whereRes;  // Очищаем память

  wprintf(L"Применяем операцию reduce - сложение для всех чисел (конкатенация для строк)\n");
  T reduceRes = Set.reduce(reduce_function);
  std::wcout << L"Результат reduce: " << reduceRes << std::endl << std::endl;

  wprintf(L"\n");
}


// Основные операции с выбранным типом данных
template <class T>
void tree_menu() {
  BinaryTree<T> bt;
  MenuItem menu[] = {
    {L"Базовые операции: вставка, поиск, удаление + Балансировка", tree_base_operations<T>},
    {L"Применение функции map, where, reduce - ручной ввод данных", apply_map_where_reduce<T>},
    //{L"Прошивка", thread<T>},
    //      {L"Извлечение подпоследовательности (по заданным индексам)", stack_getSubSequence<T>},
    //      {L"Поиск на вхождение подпоследовательности", stack_findSubSequence<T>},
    //{L"Сравнение времени добавления элементов в стек на основе LinkedList и DynamicArray", stack_addElementSpeed<T>}
  };
  menuLoop(L"Возможные операции", _countof(menu), menu);
}

template <class T>
void set_menu() {
  Set<T> st;
  MenuItem menu[] = {{L"Базовые операции: вставка, поиск, удаление", set_base_operations<T>},
       {L"Применение функции map, where, reduce - ручной ввод данных", apply_map_where_reduce_set<T>},
  //      {L"Извлечение подпоследовательности (по заданным индексам)", stack_getSubSequence<T>},
        //{L"Поиск на вхождение подпоследовательности", },
  //      {L"Сравнение времени добавления элементов в стек на основе LinkedList и DynamicArray",
  //      stack_addElementSpeed<T>}
          };
   menuLoop(L"Возможные операции", _countof(menu), menu);
}

void binaryTreeMenu() {
  wcout << endl << L"Демонстрация операций с деревом" << endl;
  MenuItem menu[] = {
    {L"Целые числа (int)", tree_menu<int>}, {L"Вещественные числа (double)", tree_menu<double>},
    // {L"Комплексные числа (complex<double>)", tree_menu<std::complex<double>>},
    //    {L"Строки/символы (string)", tree_menu<std::string>}
  };
  try {
    menuLoop(L"Выберите тип элементов", _countof(menu), menu);
  } catch (exception &ex) {
    wcout << L"Exception: " << ex.what() << endl << endl;
  }
}

void setMenu() {
  wcout << endl << L"Демонстрация операций с множеством" << endl;
  MenuItem menu[] = {{L"Целые числа (int)", set_menu<int>},
    {L"Вещественные числа (double)", set_menu<double>},
   // {L"Комплексные числа (complex<double>)", set_menu<std::complex<double>>},
    //{L"Строки/символы (string)", set_menu<std::wstring>}
  };
  try {
    menuLoop(L"Выберите тип элементов", _countof(menu), menu);
  } catch (exception &ex) {
    wcout << L"Exception: " << ex.what() << endl << endl;
  }
}

// Основная программа
int main() {
  // Задаём кодировку UTF-16 для всего вывода в программе
  // Все символы и строки будут wchar_t
#if WIN32 || WIN64
  _setmode(_fileno(stdout), _O_U16TEXT);
  _setmode(_fileno(stdin), _O_U16TEXT);
  _setmode(_fileno(stderr), _O_U16TEXT);
#endif
  wprintf(L"== Тестирование операций с деревом и множеством ==\n");

  MenuItem menu[] = {{L"Работа с бинарным деревом поиска", binaryTreeMenu}, {L"Работа с множеством", setMenu}};
  try {
    menuLoop(L"Выберите с какой структурой данных будем работать", _countof(menu), menu);
  } catch (exception &ex) {
    wcout << L"Exception: " << ex.what() << endl << endl;
  }
}
