#ifndef _PRT_SERVER_H
#define _PRT_SERVER_H

#include "functional"

#include "mocutils/channel.h"

#include "package.h"
#include "define.h"

namespace prt {
class server {
 public:
  server(int port);
  void start();
  bool add_router(std::string identifier, std::function<bytes(int, bytes)> handler);
  void tell(int client_id, std::string identifier, bytes body);
  bytes promise(int client_id, std::string identifer, bytes body);
};
};  // namespace prt

#endif