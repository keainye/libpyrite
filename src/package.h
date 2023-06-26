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
};
}  // namespace prt

#endif