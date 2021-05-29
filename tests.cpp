#include <tree.h>

#include <chrono>
#include <complex>
#include <cstdlib>

#include "binaryheap.h"
#include "binarytree.h"
#include "gtest/gtest.h"
#include "set.h"

using namespace std;

// Для примера - функция которая возводит числа в квадрат
constexpr int square(int x) {
  return x * x;
};
// Инкремент
constexpr int inc_int(int x) {
  return x + 1;
};
// Декремент
constexpr int dec_int(int x) {
  return x - 1;
};

// Возведение вещественного (с плавающей точкой) числа в квадрат
double square_double(double x) {
  return x * x;
};

complex<double> square_complex(complex<double> x) {
  return x * x;
};

// Является ли число чётным?
constexpr bool isEven(int x) {
  return (x % 2) == 0;
}

// Сумма двух целых чисел
constexpr int sum(int a, int b) {
  return a + b;
}

TEST(Tree, reduce) {  // Элементы: Целые числа
  static_assert(sum(1, 2) == 3, "1 + 2 = 3");
  static_assert(sum(11, 22) == 33, "11 + 22 = 33");
  static_assert(sum(1000, -203) == 797, "1000 - 203 = 797");
}

// Сравниваем итератор с известным итератором (вектора)
void checkIterator(BinaryTree<int> &bt, vector<int> vec) {
  ASSERT_EQ(vec.size(), bt.getSize());
  ASSERT_EQ(vec.size(), bt.calcSize());
  auto itVec = vec.begin();
  auto it = bt.begin();
  for (int i = 0; i < vec.size(); i++) {
    ASSERT_EQ(*itVec, *it);
    itVec++;
    it++;
  }
  ASSERT_EQ(vec.end(), itVec);
  ASSERT_EQ(bt.end(), it);
}

// Пример дерева поиска из Википедии
// https://ru.wikipedia.org/wiki/%D0%94%D0%B2%D0%BE%D0%B8%D1%87%D0%BD%D0%BE%D0%B5_%D0%B4%D0%B5%D1%80%D0%B5%D0%B2%D0%BE_%D0%BF%D0%BE%D0%B8%D1%81%D0%BA%D0%B0
TEST(BinaryTree, small_tree) {
  BinaryTree<int> bt;
  ASSERT_EQ(0, bt.getSize());
  bt.insert(8);
  ASSERT_EQ(string("8"), bt.toLNR());
  ASSERT_EQ(string("8"), bt.toString("N L R"));
  ASSERT_EQ(8, bt.reduce(sum));
  bt.insert(3);
  ASSERT_EQ(string("3 8"), bt.toLNR());
  ASSERT_EQ(string("3 8"), bt.toString("L N R"));
  ASSERT_EQ(3 + 8, bt.reduce(sum));
  bt.insert(10);
  checkIterator(bt, vector<int>({8, 3, 10}));
  ASSERT_EQ(2, bt.height());
  // Сохранение в строку
  // - по фиксированному обходу
  // - по обходу, задаваемому строкой форматирования (например: «{К}(Л)[П]»)
  // 1. КЛП = Корень Левый Правый
  ASSERT_EQ(string("8 3 10"), bt.toNLR());
  ASSERT_EQ(string("8 3 10"), bt.toString("N L R"));
  ASSERT_EQ(string("8-3+10"), bt.toString("N-L+R"));
  ASSERT_EQ(string("(8)[3]{10}"), bt.toString("(N)[L]{R}"));
  // 2. КПЛ = Корень Правый Левый
  ASSERT_EQ(string("8 10 3"), bt.toNRL());
  ASSERT_EQ(string("8 10 3"), bt.toString("N R L"));
  // 3. ЛПК = Левый Правый Корень
  ASSERT_EQ(string("3 10 8"), bt.toLRN());
  // 4. ЛКП = Левый Корень Правый
  ASSERT_EQ(string("3 8 10"), bt.toLNR());
  // 5. ПЛК = Правый Левый Корень
  ASSERT_EQ(string("10 3 8"), bt.toRLN());
  // 6. ПКЛ = Правый Корень Левый
  ASSERT_EQ(string("10 8 3"), bt.toRNL());
  bt.insert(1);
  ASSERT_EQ(string("1 3 8 10"), bt.toLNR());
  ASSERT_EQ(string("1 3  8 10"), bt.toString("L N R"));
  checkIterator(bt, vector<int>({8, 3, 1, 10}));
  bt.insert(6);
  ASSERT_EQ(string("1 3 6 8 10"), bt.toLNR());
  bt.insert(4);
  ASSERT_EQ(string("1 3 4 6 8 10"), bt.toLNR());
  bt.insert(7);
  ASSERT_EQ(string("1 3 4 6 7 8 10"), bt.toLNR());
  bt.insert(14);
  ASSERT_EQ(string("1 3 4 6 7 8 10 14"), bt.toLNR());
  bt.insert(13);
  ASSERT_EQ(string("1 3 4 6 7 8 10 13 14"), bt.toLNR());
  ASSERT_EQ(string("6 3 1 4 8 7 13 10 14"), bt.toNLR());
  checkIterator(bt, vector<int>({6, 3, 1, 4, 8, 7, 13, 10, 14}));
  // Извлечение поддерева (по заданному корню)
  BinaryTree<int> *subTree = bt.subTree(13);
  ASSERT_EQ(string("13 10 14"), subTree->toNLR());
  // Сравнение деревьев
  int data[] = {14};
  BinaryTree<int> bt2(data, 1);
  ASSERT_FALSE(subTree->match(bt2));
  bt2.insert(10);
  bt2.insert(13);
  ASSERT_EQ(string("13 10 14"), bt2.toNLR());
  ASSERT_TRUE(subTree->match(bt2));
  // Поиск на вхождение поддерева
  ASSERT_TRUE(bt.subTreeCheck(subTree));
  delete subTree;
}

TEST(BinaryTree, iterator) {
  BinaryTree<int> bt;
  ASSERT_EQ(0, bt.getSize());
  bt.insert(8);
  ASSERT_EQ("8", bt.toLNR());
  ASSERT_EQ("8", bt.toString("N L R"));
  bt.insert(3);
  ASSERT_EQ("3 8", bt.toLNR());
  ASSERT_EQ("3 8", bt.toString("L N R"));
  bt.insert(10);
  ASSERT_EQ(3, bt.getSize());
  //   8
  // 3  10
  // Создаём вектор с заданным обходом
  vector<int> vec = {8, 3, 10};
  ASSERT_EQ(3, vec.size());
  ASSERT_EQ("3 8 10", bt.toLNR());
  // Прошивка
  // - по фиксированному обходу
  // - по обходу, задаваемому параметром метода
  BinaryTree<int>::Node *first = bt.thread();
  ASSERT_EQ(8, first->value);
  ASSERT_EQ(3, first->next->value);
  ASSERT_EQ(10, first->next->next->value);
  ASSERT_EQ(nullptr, first->next->next->next);
  BinaryTree<int>::Node *lnr = bt.thread("LNR");
  ASSERT_EQ(3, lnr->value);
  ASSERT_EQ(8, lnr->next->value);
  ASSERT_EQ(10, lnr->next->next->value);
  ASSERT_EQ(nullptr, lnr->next->next->next);
  // map - применение операции к каждому элементу
  BinaryTree<int> *sq = bt.map(square);
  bt.printAsTree();
  ASSERT_EQ("9 64 100", sq->toLNR());
  delete sq;
  // where
  BinaryTree<int> *sq2 = bt.where(isEven);
  ASSERT_EQ(string("8 10"), sq2->toLNR());
  delete sq2;
  // reduce
  ASSERT_EQ(3 + 8 + 10, bt.reduce(sum));
}

// Базовые операции с деревом: вставка, поиск, удаление
TEST(BinaryTree, base_operations_insert_find_remove) {
  BinaryTree<int> bt;
  ASSERT_EQ(0, bt.getSize());
  // Будем добавлять одно и то же в дерево поиска и в вектор
  // Добавляем значения и проверяем как они добавились
  vector<int> vec;  // Вектор TODO: set
  const int SIZE = 1000;
  for (int i = 0; i < SIZE; i++) {
    // Генерируем новое значение
    int value = (i * 17 + 13) % SIZE;
    ASSERT_FALSE(bt.find(value));
    bt.insert(value);      // Вставка - добавляем узел в наше дерево
    vec.push_back(value);  // И одновременно добавляем в вектор
    ASSERT_TRUE(bt.find(value));
    ASSERT_EQ(vec.size(), bt.getSize());  // Размеры должны быть одинаковые
    for (int x : vec) {
      ASSERT_TRUE(bt.find(x));  // Поиск
    }
    //  Минимальный и максимальный элемент
    auto minValue = *min_element(vec.begin(), vec.end());
    auto maxValue = *max_element(vec.begin(), vec.end());
    ASSERT_EQ(minValue, bt.minimum()->value);
    ASSERT_EQ(minValue, bt.getMin(bt.getRoot()));
    ASSERT_EQ(maxValue, bt.maximum()->value);
    ASSERT_EQ(maxValue, bt.getMax(bt.getRoot()));
    // Тестируем удаление элементов по значению
    while (bt.getSize() > (rand() % 100)) {
      // Берём случайный элемент из вектора
      int randomIndex = rand() % bt.getSize();
      int v = vec[randomIndex];
      bt.remove(v);                          // Удаление
      vec.erase(vec.begin() + randomIndex);  // Удаляем так же из вектора
      ASSERT_EQ(vec.size(), bt.getSize());   // Размеры должны быть одинаковые
      ASSERT_EQ(vec.size(), bt.calcSize());
      for (int x : vec) {
        ASSERT_TRUE(bt.find(x));
      }
    }
  }
}

// Балансировка
TEST(BinaryTree, leftRotation) {
  BinaryTree<double> bt{3, 2, 1};
  // Автоматически прошёл баланс - Малое правое вращение
  ASSERT_EQ(2, bt.height(bt.getRoot()));
  ASSERT_EQ(0, bt.disbalance_check(bt.getRoot()));
  // Дерево должно выглядеть так:
  //  2
  // 1 3
  vector<BinaryTree<double>::Node *> v = bt.threadAsVector();
  ASSERT_EQ(3, v.size());
  ASSERT_EQ(2.0, v[0]->value);
  ASSERT_EQ(1.0, v[1]->value);
  ASSERT_EQ(3.0, v[2]->value);
  //      2.000000
  // 1.000000 3.000000
  bt.printAsTree();
  bt.insert(0);
  bt.check(bt.getRoot());
  //   2
  //  1 3
  // 0
  ASSERT_EQ(3, bt.height(bt.getRoot()));
  ASSERT_EQ(1, bt.disbalance_check(bt.getRoot()));
  bt.insert(1.5);
  bt.check(bt.getRoot());
  //    2
  //  1   3
  // 0 1.5
  ASSERT_EQ(3, bt.height(bt.getRoot()));
  ASSERT_EQ(1, bt.disbalance_check(bt.getRoot()));
}

TEST(BinaryTree, printAsTree) {
  BinaryTree<int> bt{3, 2, 1};
  ASSERT_EQ(2, bt.height(bt.getRoot()));
  ASSERT_EQ(0, bt.disbalance_check(bt.getRoot()));
  bt.check(bt.getRoot());
  ASSERT_EQ(2, bt.height(bt.getRoot()));
  ASSERT_EQ(0, bt.disbalance_check(bt.getRoot()));
  // Дерево должно выглядеть так:
  //  2
  // 1 3
  bt.printAsTree();
  bt.insert(0);
  bt.check();
  //   2
  //  1 3
  // 0
  ASSERT_EQ(3, bt.height(bt.getRoot()));
  ASSERT_EQ(1, bt.disbalance_check(bt.getRoot()));
  bt.insert(4);
  bt.check();
  //    2
  //  1   3
  // 0     4
  ASSERT_EQ(3, bt.height(bt.getRoot()));
  ASSERT_EQ(0, bt.disbalance_check(bt.getRoot()));
  bt.printAsTree();
}

template <class T>
void checkHeap(MinHeap<T> &heap) {
  for (int i = 1; i < heap.getSize(); i++) {
    int p = heap.parent(i);
    ASSERT_LE(heap[p], heap[i]);
  }
}

// Бинарная куча
// Базовые операции: вставка, поиск, удаление
TEST(MinHeap, insert) {
  MinHeap<int> heap(1000);
  int minValue = INT32_MAX;
  for (int i = 0; i < 1000; i++) {
    int value = rand() % 10000;
    if (heap.find(value)) {
      continue;
    }
    heap.insert(value);
    ASSERT_TRUE(heap.find(value));
    minValue = min(minValue, value);
    ASSERT_EQ(minValue, heap.getMin());
    checkHeap(heap);
  }
}

// Варианты реализации:
// 	через указатели на узлы
// 	через массив
// Извлечение поддерева (по заданному элементу)
// Поиск на вхождение поддерева
// Сохранение в строку
// 	по фиксированному обходу
// по обходу, задаваемому строкой форматирования (наример: «{К}(Л)[П]»)
// в формате списка пар «узел-родитель»
// Чтение из строки
// 	по фиксированному обходу
// по обходу, задаваемому строкой форматирования (наример: «{К}(Л)[П]»)
// в формате списка пар «узел-родитель»

// 3-арное дерево
// Базовые операции: вставка, поиск, удаление
// map, reduce
// Извлечение поддерева (по заданному элементу)
// Поиск на вхождение поддерева
// Сохранение в строку
//    	по фиксированному обходу
// по обходу, задаваемому строкой форматирования (наример: «{К}(1)[2]{3}»)
// в формате списка пар «узел-родитель»
// Чтение из строки
// 	по фиксированному обходу
// по обходу, задаваемому строкой форматирования (например: «{К}(1)[2]{3}»)
// в формате списка пар «узел-родитель»
// Поиск узла по заданному полному (абсолютному) пути, поиск по относительному пути
// Реализация дерева поиска

// == Множество ==
void assertEquals(set<int> &check, Set<int> &s) {
  ASSERT_EQ(check.size(), s.size());  // Размеры множеств должны быть одинаковы
  for (int x : check) ASSERT_TRUE(s.find(x));
  for (int x : s) ASSERT_TRUE(check.find(x) != check.end());
}

// Базовые операции: вставка, поиск, удаление
TEST(Set, basic_operations) {
  Set<int> s;      // Наша реализация множества
  set<int> check;  // Контрольное множество
  for (int i = 0; i < 200; i++) {
    int value = rand() % 1000;               // Генерируем случайное значение
    if (check.find(value) == check.end()) {  // Если значения нет в контрольном множестве
      ASSERT_FALSE(s.find(value));           // Не должно быть и в нашем множестве
      check.insert(value);                   // Добавляем в контрольное множество
      ASSERT_TRUE(check.find(value) != check.end());
      s.insert(value);  // Добавляем в наше множество
      ASSERT_TRUE(s.find(value));
    } else {
      check.erase(value);  // Удаляем значение из контрольного множества
      ASSERT_EQ(check.find(value), check.end());  // И его больше нет
      s.erase(value);                             // Удаляем из нашего множества
      ASSERT_FALSE(s.find(value));
    }
    assertEquals(check, s);
  }
}

// Объединение двух STL множеств
template <typename T>
set<T> setUnion(const set<T> &a, const set<T> &b) {
  set<T> result = a;
  result.insert(b.begin(), b.end());
  /// set_union(a.begin(), a.end(), b.begin(), b.end(), inserter(result, result.begin()));
  return result;
}

// Пересечение двух STL множеств
template <typename T>
set<T> setIntersection(const set<T> &a, const set<T> &b) {
  set<T> result;
  set_intersection(a.begin(), a.end(), b.begin(), b.end(), inserter(result, result.begin()));
  return result;
}

// Вычитание двух STL множеств
template <typename T>
set<T> setDifference(const set<T> &a, const set<T> &b) {
  set<T> result;
  set_difference(a.begin(), a.end(), b.begin(), b.end(), inserter(result, result.end()));
  return result;
}

// Операции над множествами: объединение, пересечение, вычитание
TEST(Set, union_intersect_diff) {
  set<int> a{1, 3, 4};
  set<int> b{5, 4, 6};
  for (int i = 0; i < 20; i++) {
    a.insert(rand() % 100);
    b.insert(rand() % 100);
    Set<int> as(a);
    Set<int> bs(b);
    {
      // Объединение множеств
      set<int> check = setUnion(a, b);
      Set<int> result = as.setUnion(bs);
      assertEquals(check, result);
    }
    {
      // Пересечение множеств
      set<int> check = setIntersection(a, b);
      Set<int> result = as.intersection(bs);
      assertEquals(check, result);
    }
    {
      // Вычитание множеств
      set<int> check = setDifference(a, b);
      Set<int> result = as.difference(bs);
      assertEquals(check, result);
    }
  }
}

// map, reduce, where
// map - применение функции к каждому элементу множества
TEST(Set, map) {
  Set<int> s{3, 5, 7};
  Set<int> res = s.map(square);
  ASSERT_EQ(3, res.size());
  ASSERT_TRUE(res.find(3 * 3));
  ASSERT_TRUE(res.find(5 * 5));
  ASSERT_TRUE(res.find(7 * 7));
  ASSERT_FALSE(res.find(7 * 7 + 2));
}

// Проверка на включение (подмножества), на равенство (двух множеств)
TEST(Set, subset) {
  Set<int> A{1, 3, 4};
  Set<int> B{1, 3, 5, 4, 6};
  ASSERT_TRUE(A.subSet(B));
  ASSERT_FALSE(B.subSet(A));
  ASSERT_TRUE(Set<int>({1, 3, 4}).subSet(B));
  ASSERT_TRUE(Set<int>({3, 4}).subSet(B));
}

// Проверка на равенство (двух множеств)
TEST(Set, equal) {
  ASSERT_TRUE(Set<int>({1, 4, 3}).equal(Set<int> {1, 3, 4}));
  ASSERT_FALSE(Set<int>({1, 4, 3}).equal(Set<int> {1, 5, 6}));
}

// Сохранение в строку и чтение из строки
TEST(Set, string) {
  Set<int> as{1, 4, 3};
  ASSERT_EQ(string("1 3 4"), as.toString());
  as.insert(-10);
  ASSERT_EQ(string("-10 1 3 4"), as.toString());

  Set<int> bs("-10 1 3 4");
  ASSERT_EQ(string("-10 1 3 4"), bs.toString());
  Set<int> emptySet("");
  ASSERT_EQ(string(""), emptySet.toString());
}

// Варианты реализации:
// на базе бинарной кучи
// на базе бинарного дерева поиска
// на базе 3-арного дерева
// на базе n-арного дерева
// map, reduce, where

// Реализация общих интерфейсов (см. ЛР-2)
// ICollection
// IEnumerable, реализация TreeEnumerator
// Перегрузка операторов

// Базовые операции с деревом
TEST(Tree, base_operations) {
  // int n = 2;  // Пусть будет бинарное дерево для начала
  // auto tree = new Tree<int>(3);
  auto tree = new Tree<int, 3>();
  // auto tree = new Tree<int, 3>(n);
  //  Базовые операции: вставка, поиск, удаление
  set<int> a;
  for (int value = 1; value <= 200; value++) {
    EXPECT_FALSE(tree->find(value));
    assert(a.find(value) == a.end());
    a.insert(value);
    tree->insert(value);
    // Поиск по значению
    EXPECT_TRUE(tree->find(value));
  }
  delete tree;
}
