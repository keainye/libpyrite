#ifndef _PRT_BYTE
#define _PRT_BYTE

#include "iostream"
#include "vector"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

namespace prt {
typedef unsigned char byte;
typedef std::vector<byte> bytes;

bytes range(bytes& src, int start, int end) {
  if (end <= start) return bytes();
  int len = end-start;
  bytes ret(len);
  for (int i = 0; i < len; i++)
    ret[i] = src[i];
  return ret;
}

std::string to_string(bytes& src) {
  int end = src.size();
  for (int i = 0; i < src.size(); i++) {
    if (!src[i]) {
      end = i;
      break;
    }
  }
  char str[end+1];
  for (int i = 0; i < end; i++)
    str[i] = src[i];
  str[end] = 0;
  return std::string(str);
}

bytes to_bytes(const std::string& str) {
  bytes ret(str.size());
  for (int i = 0; i < str.size(); i++)
    ret[i] = str[i];
  return ret;
}

bytes operator+(const bytes& v1, const bytes& v2) {
  bytes ret(v1.size()+v2.size());
  int ptr = 0;
  for (int i = 0; i < v1.size(); i++)
    ret[ptr++] = v1[i];
  for (int i = 0; i < v2.size(); i++)
    ret[ptr++] = v2[i];
  return ret;
}
}  // namespace prt

#endif