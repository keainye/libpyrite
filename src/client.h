#ifndef _PRT_CLIENT_H
#define _PRT_CLIENT_H

#include "define.h"
#include "package.h"

namespace prt {
class client {
 public:
  client(const char* ip, int port, int timeout);
  ~client();
  int start();
  bool add_router(std::string identifier, std::function<bytes(bytes)> handler);
  int tell(std::string identifier, bytes body);
  bytes promise(std::string identifer, bytes body);
};
}  // namespace prt

#endif