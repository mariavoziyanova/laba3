#pragma once

#include <algorithm>
#include <cassert>
#include <codecvt>
#include <cstring>
#include <iostream>
#include <locale>
#include <set>
#include <vector>

#include "common.hpp"

#ifdef DEBUG_BUILD
#define CHECK(tree) check(tree)
#else
#define CHECK(tree)  // Ничего не делаем
#endif

using namespace std;

// Бинарное дерево поиска
// АВЛ-дерево - сбалансированное по высоте двоичное дерево поиска:
// для каждой его вершины высота её двух поддеревьев различается не более чем на 1.
// АВЛ — аббревиатура, образованная первыми буквами фамилий создателей (советских учёных):
// Георгия Максимовича Адельсон-Вельского и Евгения Михайловича Ландиса
template <typename T>
struct BinaryTree {
  struct Node;        // Узел дерева
  struct Operation {  // Абстрактный класс - операция которую можно проделать с узлом дерева
    virtual void apply(Node *n) = 0;  // n - узел
  };
  // Узел дерева
  struct Node {
    T value;                // Данные в узле
    Node *left = nullptr;   // Левое поддерево
    Node *right = nullptr;  // Правое поддерево
    Node *next = nullptr;  // Для прошивки дерева: следующая вершина в порядке обхода дерева
    int height = 1;  // Высота поддерева с корнем в этой вершине
    int dis = 0;     // Дисбаланс (+1, 0, -1 - для сбалансированного дерева - AVL)
    // Если меняем что-то в поддеревьях данной вершины, то высота и дисбаланс могут меняться
    // Пересчитываем высоту и дисбаланс считая что для поддеревьев уже подсчитано
    void reCalc() {
      int leftHeight = (left) ? left->height : 0;
      int rightHeight = (right) ? right->height : 0;
      height = std::max(leftHeight, rightHeight) + 1;  // Высота поддерева с корнем в этой вершине
      dis = leftHeight - rightHeight;  // Запоминаем дисбаланс для данной вершины
    }
    // Создание узла, параметры: значение и родитель
    explicit Node(T value, Node *left = nullptr, Node *right = nullptr) : value(value), left(left), right(right) {
      reCalc();
    }
    // == Обходы ==
    // 1. КЛП = Корень Левый Правый
    void NLR(Operation &op) {
      op.apply(this);
      if (left) left->NLR(op);
      if (right) right->NLR(op);
    }
    // 2. КПЛ = Корень Правый Левый
    void NRL(Operation &op) {
      op.apply(this);
      if (right) right->NRL(op);
      if (left) left->NRL(op);
    }
    // 3. ЛПК = Левый Правый Корень
    void LRN(Operation &op) {
      if (left) left->LRN(op);
      if (right) right->LRN(op);
      op.apply(this);
    }
    // 4. ЛКП = Левый Корень Правый
    void LNR(Operation &op) {
      if (left) left->LNR(op);
      op.apply(this);
      if (right) right->LNR(op);
    }
    // 5. ПЛК = Правый Левый Корень
    void RLN(Operation &op) {
      if (right) right->RLN(op);
      if (left) left->RLN(op);
      op.apply(this);
    }
    // 6. ПКЛ = Правый Корень Левый
    void RNL(Operation &op) {
      if (right) right->RNL(op);
      op.apply(this);
      if (left) left->RNL(op);
    }
    void print(const char *format, std::ostream &os) {
      if (!left && !right) {
        os << value;
        return;
      }
      for (size_t i = 0, len = strlen(format); i < len; i++) {
        switch (format[i]) {
        case 'N':
          os << value;
          break;
        case 'L':
          if (left) left->print(format, os);
          break;
        case 'R':
          if (right) right->print(format, os);
          break;
        default:
          os << format[i];
        }
      }
    }
  };

 private:
  Node *root = nullptr;  // Корень дерева
  int size = 0;          // Количество узлов в дереве
  // Рекурсивное удаление дерева со всеми поддеревьями
  void delTree(Node *tree) {
    if (tree == nullptr) return;
    delTree(tree->left);
    delTree(tree->right);
    delete tree;
  }
  // Проходим в порядке: Корень Левый Правый
  void NLR(Node *n, Operation &op) {
    if (n == nullptr) return;
    op.apply(n);        // Применяем операцию к корню
    NLR(n->left, op);   // Вызываем обход для левого
    NLR(n->right, op);  // Вызываем обход для правого
  }
  // Обход в заданным строкой порядке с применением операции
  void go(Node *n, const char *order, Operation &op) {
    if (n == nullptr) return;
    for (size_t i = 0, len = strlen(order); i < len; i++) {
      switch (order[i]) {
      case 'L':
        go(n->left, order, op);
        break;
      case 'R':
        go(n->right, order, op);
        break;
      case 'N':
        op.apply(n);
        break;
      }
    }
  }
  // Копирование поддерева
  Node *copy(Node *n) {
    if (n == nullptr) return nullptr;
    return new Node(n->value, copy(n->left), copy(n->right));
  }
  // Вставка: добавляем вершину в дерево поиска
  // n - корень поддерева куда добавляем
  // v - добавляемое значение
  Node *insertTo(Node *n, const T &v) {  // Добавляемое значение
    if (v <= n->value) {  // Если значение <= значению в узле, добавляем в левое поддерево
      if (n->left)                       // Если левое поддерево уже есть
        n->left = insertTo(n->left, v);  // Тогда добавляем в него
      else
        n->left = new Node(v);  // Создаём новый узел со значением v
    } else {                    // Если больше, то добавляем вправо
      if (n->right) {           // Если правое поддерево уже есть
        n->right = insertTo(n->right, v);
      } else
        n->right = new Node(v);
    }
    return balance(n);  // Чтобы дерево оставалось сбалансированным
  }
  explicit BinaryTree(Node *root) : root(root) {
    this->size = subTreeSize(root);
    check(root);
  }

 public:
  BinaryTree() = default;
  BinaryTree(const T *items, const int size) {
    for (int i = 0; i < size; i++) insert(items[i]);
  }
  explicit BinaryTree(const std::set<T> &set) {
    for (T x : set) insert(x);
  }
  BinaryTree(initializer_list<T> list) {
    for (T x : list) insert(x);
  }
  ~BinaryTree() {
    delTree(root);
  }
  int getSize() const {
    return size;
  }
  Node *getRoot() {
    return root;
  }
  // Базовые операции: вставка, поиск, удаление
  // Вставка: добавить значение в двоичное дерево поиска
  void insert(const T &value) {
    size++;  // Увеличиваем размер дерева
    if (root) {
      root = insertTo(root, value);
    } else
      root = new Node(value);
  }
  // Поиск узла по значению
  Node *find(const T &v) const {
    Node *n = root;         // Начинаем с корня дерева
    while (n != nullptr) {  // Пока указатель не NULL
      // Если значение равно, то возвращаем найденный узел
      if (v == n->value) return n;
      if (v < n->value)  // Если меньше, идём влево
        n = n->left;
      else  // Иначе вправо
        n = n->right;
    }
    return nullptr;  // Не нашли узла со значением v
  }
  // Удаление узла по значению
  // Входные параметры: дерево и значение которое нужно удалить
  // Возвращаем дерево с удалённым узлом (если есть)
  Node *remove(Node *r, const T &v) {
    // Пустое дерево - искать и удалять негде
    if (r == nullptr) return nullptr;
    // Если значение не равно, то есть мы ещё не нашли, идём по поддеревьям
    if (v < r->value) {  // Если значение меньше, то удаляем в левом поддереве
      r->left = remove(r->left, v);
      return balance(r);
    }
    if (v > r->value) {  // Если значение больше, то удаляем в правом поддереве
      r->right = remove(r->right, v);
      return balance(r);
    }
    // Если значение равно, то мы нашли нужный элемент
    assert(v == r->value);
    if (r->left && r->right) {  // Если есть и правое и левое поддерево, мы просто так удалить его не можем
      // Заменяем значение на минимум из правого поддерева
      r->value = minimum(r->right)->value;
      r->right = remove(r->right, r->value);  // Удаляем это значение из правого поддерева
    } else {  // Мы нашли узел для удаления и одна из веток (либо правая, либо левая) отсутствует
      Node *toDelete = r;  // Узел для удаления
      if (r->left) {       // Заменяем на левый узел
        r = r->left;
      } else if (r->right) {  // Заменяем на правый узел
        r = r->right;
      } else
        r = nullptr;
      size--;
      delete toDelete;
    }
    return balance(r);
  }
  // Удаление узла по значению
  void remove(const T &v) {
    root = remove(root, v);
  }
  // Высота дерева
  int height() const {
    return root ? root->height : 0;
  }
  // Высота дерева - считаем для проверки
  int height(Node *n) {
    if (!n) return 0;  // Для пустого дерева 0
    // Максимум из высот левого и правого + 1
    return max(height(n->left), height(n->right)) + 1;
  }
  // Минимум для данного поддерева
  Node *minimum(Node *n) {
    if (n == nullptr) throw range_error("Empty tree");
    while (n->left) n = n->left;
    return n;
  }
  Node *minimum() {
    return minimum(root);
  }
  Node *maximum(Node *n) {
    if (n == nullptr) throw range_error("Empty tree");
    while (n->right) n = n->right;
    return n;
  }
  Node *maximum() {
    return maximum(root);
  }
  T getMin(Node *n) {
    return minimum(n)->value;
  }
  T getMax(Node *n) {
    return maximum(n)->value;
  }
  // Поддерево по ключу
  BinaryTree<T> *subTree(const T &v) {
    return new BinaryTree<T>(copy(find(v)));
  }
  // Вычислить размер поддерева
  int subTreeSize(const Node *n) {
    if (n)
      return subTreeSize(n->left) + 1 + subTreeSize(n->right);
    else
      return 0;
  }
  // Посчитать размер дерева
  int calcSize() {
    return subTreeSize(root);
  }
  // Сравнение деревьев
  bool matchTree(const Node *a, const Node *b) {
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;
    if (a->value != b->value) return false;
    return matchTree(a->left, b->left) && matchTree(a->right, b->right);
  }
  bool match(BinaryTree<T> &tree) {
    return matchTree(root, tree.root);
  }
  bool subTreeCheck(BinaryTree<T> *subTree) {
    if (subTree == nullptr || subTree->root == nullptr) return false;
    Node *n = find(subTree->root->value);
    if (!n) return false;
    return matchTree(subTree->root, n);
  }
  struct Print : public Operation {
    stringstream ss;
    void apply(Node *n) override {
      ss << n->value << " ";
    }
    string result() {
      return trim_copy(ss.str());
    }
  };
  // == Обходы ==
  // 1. КЛП = Корень Левый Правый
  string toNLR() {
    Print op;
    if (root) root->NLR(op);
    return op.result();
  }
  // 2. КПЛ = Корень Правый Левый
  string toNRL() {
    Print op;
    if (root) root->NRL(op);
    return op.result();
  }
  // 3. ЛПК = Левый Правый Корень
  string toLRN() {
    Print op;
    if (root) root->LRN(op);
    return op.result();
  }
  // 4. ЛКП = Левый Корень Правый
  string toLNR() {
    Print op;
    if (root) root->LNR(op);
    return op.result();
  }
  // 5. ПЛК = Правый Левый Корень
  string toRLN() {
    Print op;
    if (root) root->RLN(op);
    return op.result();
  }
  // 6. ПКЛ = Правый Корень Левый
  string toRNL() {
    Print op;
    if (root) root->RNL(op);
    return op.result();
  }
  // Вывод строку в соответствии с форматом
  string toString(const char *format) {
    stringstream ss;
    if (root) root->print(format, ss);
    return trim_copy(ss.str());
  }
  // map, reduce, where
  // map - применение функции к каждому элементу дерево
  // Создаётся новое дерево
  BinaryTree<T> map(T f(T)) {
    BinaryTree<T> res;
    for (T x : *this) {
      res.insert(f(x));
    }
    return res;
  }
  // where фильтрует значения из списка l с помощью функции-фильтра h
  BinaryTree<T> where(bool h(T)) {
    BinaryTree<T> res;
    for (T x : *this) {
      if (h(x)) res.insert(x);
    }
    return res;
  }
  // reduce - применяем к каждой паре значений пока не получим одно значение
  T reduce(T f(T, T)) {
    return reduce(root, f);
  }
  T reduce(Node *n, T f(T, T)) {
    if (n->left) {                                // Если есть левое поддерево
      T value = f(n->value, reduce(n->left, f));  // Считаем значение для левого поддерева
      if (n->right) {                             // Если есть правое поддерево
        return f(value, reduce(n->right, f));     // Результат
      }
      return value;
    }
    if (n->right) {                             // Если есть правое поддерево
      return f(n->value, reduce(n->right, f));  // Результат
    }
    return n->value;  // Нет поддеревьев => возвращаем значение из узла
  }
  // == Прошивка ==
  // - по фиксированному обходу
  // - по обходу, задаваемому параметром метода
  // Для прошивки - начальный узел
  Node *first;
  // Прошивка
  struct Thread : public Operation {
    Node *first = nullptr;          // Первый узел в обходе
    Node *last = nullptr;           // Последний узел в обходе
    void apply(Node *n) override {  // Добавить узел в прошивку
      if (first == nullptr) first = n;  // Если это первый узел в прошивке => запоминаем как первый
      if (last != nullptr) last->next = n;  // Если был какой-то узел до этого => пришиваем к нему текущий
      n->next = nullptr;  // Стираем ссылку на следующий у текущего элемента
      // затираем старую прошивку
      last = n;
    };
  };
  // Прошивка с результатом в виде вектора
  struct ThreadVector : public Operation {
    vector<Node *> path;            // Путь по дереву
    void apply(Node *n) override {  // Добавить узел в прошивку
      path.push_back(n);
    };
  };
  // Прошивка дерева в порядке Корень Левое Правое
  Node *thread() {
    first = nullptr;
    Thread td;
    NLR(root, td);
    first = td.first;
    return first;
  }
  vector<Node *> threadAsVector() {
    ThreadVector td;
    NLR(root, td);
    return td.path;
  }
  // Прошивка дерева в заданном порядке N-Корень L-Левое R-Правое
  Node *thread(const char *order) {
    assert(strlen(order) == 3);
    first = nullptr;
    Thread td;
    go(root, order, td);
    first = td.first;
    return first;
  }
  // Проверка разницы высот для данной вершины: высота_левого - высота_правого
  int disbalance_check(Node *t) {
    if (t == nullptr) return 0;
    return height(t->left) - height(t->right);
  }
  // Проверка правильности сбалансированного дерева
  void checkBeforeBalance(Node *n) {
    assert(n->height == height(n));         // Проверяем правильность высоты
    assert(n->dis == disbalance_check(n));  // Правильность дисбаланса
    assert(n->dis <= 2);                    // Дисбаланс в корректных пределах
    assert(n->dis >= -2);
    if (n->left) {  // Проверяем те же свойства для левого
      assert(n->left->value <= n->value);
      check(n->left);
    }
    if (n->right) {  // И правого поддерева
      assert(n->value < n->right->value);
      check(n->right);
    }
  }
  void check() {
    check(root);
  }
  void check(Node *n) {
    checkBeforeBalance(n);
    assert(n->dis >= -1);
    assert(n->dis <= 1);
  }
  Node *balance(Node *a) {
    if (a == nullptr) return nullptr;
    a->reCalc();
    // DEBUG: checkBeforeBalance(a);
    if (a->dis <= -2) {
      if (a->right->dis <= 0)
        a = leftRotation(a);
      else
        a = bigLeftRotation(a);
    } else if (a->dis >= 2) {
      if (a->left->dis >= 0)
        a = rightRotation(a);
      else
        a = bigRightRotation(a);
    }
    CHECK(a);
    return a;
  }

  // Малое левое вращение
  Node *leftRotation(Node *a) {
    //   a          b
    // L  b   =>  a  R
    //   C R     L C
    // L и R остаются на своих местах:
    // L слева от a, R справа от b
    Node *b = a->right;
    a->right = b->left;  // Перевешиваем С
    a->reCalc();         // Пересчитываем высоту и дисбаланс a
    b->left = a;         // Подвешиваем старый корень a
    b->reCalc();         // Пересчитываем высоту и дисбаланс b
    return b;
  }
  // Малое правое вращение
  Node *rightRotation(Node *a) {
    //    a         b
    //  b  R  =>  L  a
    // L C          C R
    // L и R остаются на своих местах
    // L слева от b, R справа от a
    assert(a->left != nullptr);
    Node *b = a->left;
    a->left = b->right;  // Перевешиваем С
    a->reCalc();         // Пересчитываем высоту и дисбаланс a
    b->right = a;
    b->reCalc();  // Пересчитываем высоту и дисбаланс b
    return b;
  }
  // Большое левое вращение
  Node *bigLeftRotation(Node *a) {
    Node *b = a->right;
    a->right = rightRotation(b);
    return leftRotation(a);
  }
  // Большое правое вращение
  Node *bigRightRotation(Node *a) {
    Node *newRoot = a->left;
    a->left = leftRotation(newRoot);
    return rightRotation(a);
  }
  string print(Node *n) {
    if (n) {
      return to_string(n->value) + " " + to_string(n->height) + "/" + to_string(n->dis);
    } else {
      return "";
    }
  }
  // Одна клеточка для представления узла при рисовании
  struct CellDisplay {
    bool present;   // Существует ли узел?
    string valStr;  // Значение в виде строки
    explicit CellDisplay(Node *n) : present(n) {
      valStr = (n) ? to_string(n->value) : "";

    };
  };

  // Таблица с представлением ячеек
  vector<vector<CellDisplay>> getDisplayRows() const {

    vector<Node *> traversal_stack;
    if (!root) return vector<vector<CellDisplay>>();
    vector<vector<Node *>> nodes;  // Узлы дерева по уровням
    const int height = root->height;
    nodes.resize(height);
    Node *p = root;
    int depth = 0;
    while (true) {

      if (depth == height - 1) {
        nodes[depth].push_back(p);
        if (depth == 0) break;
        --depth;
        continue;
      }

      if (traversal_stack.size() == depth) {
        nodes[depth].push_back(p);
        traversal_stack.push_back(p);
        if (p) p = p->left;
        ++depth;
        continue;
      }

      if (nodes[depth + 1].size() % 2) {
        p = traversal_stack.back();
        if (p) p = p->right;
        ++depth;
        continue;
      }

      if (depth == 0) break;
      traversal_stack.pop_back();
      p = traversal_stack.back();
      --depth;
    }
    // Преобразуем Node* в CellDisplay
    vector<vector<CellDisplay>> display;
    for (const auto &row : nodes) {
      display.emplace_back();  // Создаём новую строчку
      for (Node *n : row) display.back().push_back(CellDisplay(n));
    }
    return display;
  }


  vector<string> row_formatter(const vector<vector<CellDisplay>> &rows) const {
    using s_t = string::size_type;

    s_t cell_width = 0;
    for (const auto &row : rows)
      for (const auto &cd : row)
        if (cd.present && cd.valStr.length() > cell_width) {
          cell_width = cd.valStr.length();
        };

    if (cell_width % 2 == 0) ++cell_width;

    vector<string> formatted_rows;

    s_t row_count = rows.size();

    s_t row_elem_count = 1 << (row_count - 1);

    s_t left_pad = 0;

    for (s_t r = 0; r < row_count; ++r) {
      const auto &cd_row = rows[row_count - r - 1];

      s_t space = (s_t(1) << r) * (cell_width + 1) / 2 - 1;

      string row;

      for (s_t c = 0; c < row_elem_count; ++c) {

        row += string(c ? left_pad * 2 + 1 : left_pad, ' ');
        if (cd_row[c].present) {

          const string &valstr = cd_row[c].valStr;

          s_t long_padding = cell_width - valstr.length();
          s_t short_padding = long_padding / 2;
          long_padding -= short_padding;
          row += string(c % 2 ? short_padding : long_padding, ' ');
          row += valstr;
          row += string(c % 2 ? long_padding : short_padding, ' ');
        } else {

          row += string(cell_width, ' ');
        }
      }

      formatted_rows.push_back(row);

      if (row_elem_count == 1) break;

      s_t left_space = space + 1;
      s_t right_space = space - 1;
      for (s_t sr = 0; sr < space; ++sr) {
        string row;
        for (s_t c = 0; c < row_elem_count; ++c) {
          if (c % 2 == 0) {
            row += string(c ? left_space * 2 + 1 : left_space, ' ');
            row += cd_row[c].present ? '/' : ' ';
            row += string(right_space + 1, ' ');
          } else {
            row += string(right_space, ' ');
            row += cd_row[c].present ? '\\' : ' ';
          }
        }
        formatted_rows.push_back(row);
        ++left_space;
        --right_space;
      }
      left_pad += space + 1;
      row_elem_count /= 2;
    }

    std::reverse(formatted_rows.begin(), formatted_rows.end());
    return formatted_rows;
  }

  static void trim_rows_left(vector<string> &rows) {
    if (rows.empty()) return;
    auto min_space = rows.front().length();
    for (const auto &row : rows) {
      auto i = row.find_first_not_of(' ');
      if (i == string::npos) i = row.length();
      if (i == 0) return;
      if (i < min_space) min_space = i;
    }
    for (auto &row : rows) {
      row.erase(0, min_space);
    }
  }
  // Печать в виде красивого дерева с ветками
  void printAsTree() const {
    wcout << endl;
    if (root == nullptr) {  // Если дерево пустое
      wcout << " <empty tree>" << endl;
      return;
    }
    // Получаем таблицу из ячеек
    const auto rows = getDisplayRows();
    // Генерируем массив отформатированных строк
    auto formatted = row_formatter(rows);
    // Обрезаем лишние пробелы слева
    trim_rows_left(formatted);
    // Выводим на консоль как комментарии
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    // std::string narrow = converter.to_bytes(wide_utf16_source_string);
    for (const auto &row : formatted) {
      std::wstring wide = converter.from_bytes(row);
      wcout << L" " << wide << endl;
    }
  }
  // Итератор для BinaryTree
  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    // Создаём обход и запоминаем индекс
    explicit Iterator(Node *node, int index) : index(index), root(node) {
      NLR(node);
    }
    reference operator*() const {
      return path[index]->value;
    }
    pointer operator->() {
      return path[index]->value;
    }
    Iterator &operator++() {
      index++;
      if (index > path.size()) index = path.size();
      return *this;
    }
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    friend bool operator==(const Iterator &a, const Iterator &b) {
      return a.index == b.index && a.root == b.root;
    };
    friend bool operator!=(const Iterator &a, const Iterator &b) {
      return a.index != b.index || a.root != b.root;
    };

   private:
    // Node Left Right - Корень Левое Правое
    void NLR(Node *node) {
      if (node == nullptr) return;
      path.push_back(node);  // Добавляем узел в путь
      if (node->left != nullptr) NLR(node->left);
      if (node->right != nullptr) NLR(node->right);
    }
    std::vector<Node *> path;
    int index;
    Node *root;
  };
  Iterator begin() const {
    return Iterator(root, 0);
  }
  Iterator end() const {
    return Iterator(root, getSize());
  }
};
