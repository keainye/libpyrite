#include "byte.h"

prt::bytes::bytes(int _len): len(max(0, _len)) {
  this->core = nullptr;
  if (_len <= 0) return;
  this->core = new byte[_len];
}

prt::bytes::~bytes() {
  if (!this->core) return;
  delete[] this->core;
}

prt::byte& prt::bytes::operator[](int index) {
  return this->core[index];
}