#pragma once

#include <cassert>
#include <cwchar>
#include <iostream>

// Пункт меню
struct MenuItem {
  const wchar_t *name;
  void (*f)();
};

template <class T>
struct MenuItemX {
  const wchar_t *name;
  void (*f)(T &t);
};

// Вывод меню и выбор из меню + вызов функций
// На входе:
//  title - заголовок меню
//  list - пункты меню
struct Menu {
  Menu(const wchar_t *title, initializer_list<MenuItem> list) {
    vector<MenuItem> menu(list);
    assert(menu.size() >= 2);  // Хотя бы 2 варианта должно быть для выбора
    int N = menu.size();
    while (true) {
      wprintf(L"\n%ls (0 - выход):\n", title);  // Печатаем заголовок меню ls - строчка в формате юникод
      // Печатаем меню
      for (int no = 1; no <= menu.size(); no++) {  // no=1,2,3...N - номер пункта меню
        wprintf(L"  %d. %ls\n", no, menu[no - 1].name);
      }
      // Печатаем предложение о выборе
      wprintf(L"Выберите (1..%d): ", N);
      int choice;  // Выбор пользователя
      std::wcin >> choice;
      if (choice == 0) {
        wcout << L"Выход" << endl;
        return;
      }
      if (choice >= 1 && choice <= N) {
        int index = choice - 1;
        wcout << endl << L" == " << choice << L". " << menu[index].name << " ==" << endl << endl;
        try {
          menu[index].f();  // Вызываем функцию
        } catch (exception &ex) {
          wcout << L"Exception: " << ex.what() << endl << endl;
        }
        wcout << endl;
      } else {
        wprintf(L"Пункта меню %d нет => выход\n", choice);
        return;
      }
    }
  }
};

template <class T = void>
struct MenuX {
  MenuX(const wchar_t *title, initializer_list<MenuItemX<T>> list, T &t) {
    vector<MenuItemX<T>> menu(list);
    assert(menu.size() >= 2);  // Хотя бы 2 варианта должно быть для выбора
    int N = menu.size();
    while (true) {
      wcout << endl << title << L" (0 - выход из меню):" << endl;  // Печатаем заголовок меню ls - строчка в формате юникод
      // Печатаем меню
      for (int no = 1; no <= menu.size(); no++) {  // no=1,2,3...N - номер пункта меню
        wcout << L"  " << no << L". " << menu[no - 1].name << endl;
      }
      // Печатаем предложение о выборе
      wcout << L"Выберите (1.." << N << L"): ";
      int choice;  // Выбор пользователя
      std::wcin >> choice;
      if (choice == 0) {
        wcout << L"Выход" << endl;
        return;
      }
      if (choice >= 1 && choice <= N) {
        int index = choice - 1;
        wcout << L"Вы выбрали " << choice << L". " << menu[index].name << endl << endl;
        try {
          menu[index].f(t);  // Вызываем функцию
        } catch (exception &ex) {
          wcout << L"Exception: " << ex.what() << endl << endl;
        }
        wcout << endl;
      } else {
        wcout << L"Пункта меню " << choice << L" нет => выход" << endl;
        return;
      }
    }
  }
};
