#ifndef _PRT_PACKAGE
#define _PRT_PACKAGE

#include "string"
#include "byte.h"

namespace prt {
class package {
 public:
  std::string session;
  std::string identifier;
  int sequence;
  bytes body;

  package();
  bytes to_bytes();
  bool operator==(const package& other);
};

bool to_package(package& pac, bytes raw);
}  // namespace prt

#endif