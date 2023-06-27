#include "utils.h"
#include "../src/byte.h"

using namespace prt;
using namespace std;

int main() {
  bytes b(3);
  b[0] = '0';
  b[1] = '1';
  b[2] = '2';

  test(to_string(b) == "012");
  pass("to_string(bytes&)");

  test(to_bytes("012") == b);
  pass("to_bytes(std::string&)");

  test(to_bytes("012")+to_bytes("345") == to_bytes("012345"));
  pass("bytes& operator+(const bytes&, const bytes&)");

  b = to_bytes("012345");
  test(range(b, 3, 6) == to_bytes("345"));
  pass("bytes range(bytes&, int, int)");

  return 0;
}
