#ifndef _PRT_BYTE
#define _PRT_BYTE

#include "iostream"
#include "vector"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

namespace prt {
typedef unsigned char byte;
typedef std::vector<byte> bytes;

bytes range(bytes& src, int start, int end);
std::string to_string(bytes& src);
bytes to_bytes(const std::string& str);
bytes operator+(const bytes& v1, const bytes& v2);
void print_as_hex(const bytes& raw);
void println_as_hex(const bytes& raw);
}  // namespace prt

#endif