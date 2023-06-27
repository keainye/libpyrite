#include "utils.h"
#include "../src/package.h"

using namespace prt;

int main() {
  package p;
  p.session = "test session";
  p.identifier = "test identifier";
  p.sequence = 2333;
  p.body = to_bytes("test body");

  package p1;
  p1.session = "test session";
  p1.identifier = "test identifier";
  p1.sequence = 2333;
  p1.body = to_bytes("test body");

  test(p == p1);
  p1.sequence = 2334;
  test(!(p == p1));
  pass("bool package::operator==");

  test(p != p1);
  p1.sequence = 2333;
  test(!(p != p1));
  pass("bool package::operator!=");

  package p2;
  test(to_package(p2, p.to_bytes()));
  test(p == p2);
  pass("package convertion");

  return 0;
}