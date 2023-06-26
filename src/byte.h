#ifndef _PRT_BYTE
#define _PRT_BYTE

#include "iostream"
#include "vector"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

namespace prt {
typedef unsigned char byte;
typedef std::vector<byte> bytes;
// class bytes {
//  private:
//   byte* core;
//  public:
//   int len;
//   bytes();
//   bytes(int _len);
//   ~bytes();
//   byte& operator[](int index);
//   bytes operator+(bytes other);
//   bytes& operator=(bytes& other);
//   bytes range(int start, int end);
//   void print();
//   void println();
// };
}  // namespace prt

#endif