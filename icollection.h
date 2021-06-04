#pragma once

template <class T>
struct ICollection {
  virtual T Get(size_t index) const = 0;
  virtual size_t GetCount() const = 0;
};
