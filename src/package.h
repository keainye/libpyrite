#ifndef _PRT_PACKAGE
#define _PRT_PACKAGE

#include "string"

#include "mocutils/byte.h"

#include "define.h"

namespace prt {
typedef moc::bytes bytes;
class package {
 public:
  i32 sequence;
  std::string identifier;
  bytes body;

  package();
  package(bytes raw);
  package(i32 sequence, std::string identifier, bytes body);
  bytes to_bytes();
  bool operator==(const package& other);
  bool operator!=(const package& other);

  void set_body(std::string text);
  std::string body_as_string();
  void send_to(int connection);
};
}  // namespace prt

#endif