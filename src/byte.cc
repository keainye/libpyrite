#include "byte.h"

prt::bytes::bytes(): len(0), core(nullptr) {}

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

prt::bytes prt::bytes::operator+(bytes other) {
  bytes ret(this->len + other.len);
  int ptr = 0;
  for (int i = 0; i < this->len; i++)
    ret[ptr++] = this->core[i];
  for (int i = 0; i < other.len; i++)
    ret[ptr++] = other[i];
  return ret;
}

prt::bytes& prt::bytes::operator=(bytes& other) {
  this->len = other.len;
  delete[] this->core;
  this->core = new byte[this->len];
  for (int i = 0; i < this->len; i++)
    this->core[i] = other[i];
  return *this;
}

prt::bytes prt::bytes::range(int start, int end) {
  if (end <= start) return bytes();
  int retlen = end-start;
  bytes ret(retlen);
  for (int i = 0; i < retlen; i++)
    ret[i] = this->core[start+i];
  return ret;
}

void prt::bytes::print() {
  for (int i = 0; i < this->len; i++)
    std::printf("%02x", this->core[i]);
}

void prt::bytes::println() {
  this->print();
  std::printf("\n");
}
