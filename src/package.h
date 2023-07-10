#ifndef _PRT_PACKAGE
#define _PRT_PACKAGE

#include "string"
#include "mocutils/byte.h"
#include "winsock2.h"
#include "define.h"

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
  package(std::string session, std::string identifier, int sequence, bytes body);
  bytes to_bytes();
  bool operator==(const package& other);
  bool operator!=(const package& other);

  void set_body(std::string text);
  std::string body_as_string();
  void send_to(SOCKET connection);
};
}  // namespace prt

#endif