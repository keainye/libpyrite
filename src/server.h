#ifndef _PRT_SERVER_H
#define _PRT_SERVER_H

#include "define.h"
#include "functional"
#include "map"
#include "mocutils/channel.h"
#include "package.h"
#include "winsock2.h"

namespace prt {
class client_data {
 public:
  sockaddr_in addr;
  i64 last_acpt;
  i32 seq;
  std::map<i32, moc::channel<package>> promises;
};

class server {
  struct sockaddr_in addr;
  int port;
  i64 max_lifetime;
  std::map<std::string, std::function<bytes(sockaddr_in, bytes)>> router;
  i64 timeout;
  std::map<sockaddr_in, client_data> clients;

 public:
  server(int _port, i64 _max_lifetime, i64 _timeout);
  void start();
  void process(sockaddr_in client, package recv);
  bool add_router(std::string identifier, std::function<bytes(sockaddr_in, bytes)> ctrler);
  void tell(sockaddr_in client, std::string identifier, bytes body);
};
};  // namespace prt

#endif