#ifndef _PRT_BYTE
#define _PRT_BYTE

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

namespace prt {
typedef unsigned char byte;
class bytes {
 private:
  byte* core;
 public:
  const int len;
  bytes(int _len);
  ~bytes();
};
}  // namespace prt

#endif