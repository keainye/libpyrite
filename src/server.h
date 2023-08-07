#ifndef _PRT_SERVER_H
#define _PRT_SERVER_H

#include "define.h"
#include "map"
#include "mocutils/channel.h"
#include "package.h"
#include "winsock2.h"
#include "functional"

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
  std::map<std::string, std::function<bytes(bytes)>> router;
  i64 timeout;
  std::map<sockaddr_in, client_data> clients;
};
};  // namespace prt

#endif