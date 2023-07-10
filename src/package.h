#ifndef _PRT_PACKAGE
#define _PRT_PACKAGE

#include "string"
#include "mocutils/byte.h"

namespace prt {
typedef moc::bytes bytes;
class package {
 public:
  std::string session;
  std::string identifier;
  int sequence;
  bytes body;

  package();
  package(bytes raw);
  bytes to_bytes();
  bool operator==(const package& other);
  bool operator!=(const package& other);

  void set_body(std::string text);
  std::string body_as_string();
};
}  // namespace prt

#endif