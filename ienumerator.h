#pragma once

template <typename T>
struct IEnumerator {
  virtual T GetCurrent() = 0;  // Возвращает элемент коллекции, соответствующий текущей позиции перечислителя
  virtual bool MoveNext() = 0;  // Перемещает перечислитель к следующему элементу коллекции
  virtual void Reset() = 0;  // Устанавливает перечислитель в его начальное положение,
  // т.е. перед первым элементом коллекции
};

template <typename T>
struct IEnumerable {
  virtual IEnumerator<T> *GetEnumerator() = 0;
};
