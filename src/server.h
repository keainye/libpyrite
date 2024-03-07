#ifndef _PRT_SERVER_H
#define _PRT_SERVER_H

#include "functional"

#include "mocutils/channel.h"

#include "package.h"
#include "define.h"

namespace prt {
class server {
 public:
  server(int port, i64 timeout);
  void start();
  bool add_router(std::string identifier, std::function<bytes(sockaddr_in, bytes)> ctrler);
  void tell(sockaddr_in client, std::string identifier, bytes body);
  bytes promise(sockaddr_in client, std::string identifer, bytes body);
};
};  // namespace prt

#endif