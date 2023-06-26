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
}  // namespace prt

#endif