#ifndef _PRT_SERVER_H
#define _PRT_SERVER_H

#include "functional"

#include "mocutils/channel.h"

#include "package.h"
#include "define.h"

namespace prt {
class server {
  int server_fd, sequence;
  sockaddr_in server_addr;
  connection_state state;
 public:
  server(int port);
  void start();
  bool set_handler(std::string identifier, std::function<bytes(sockaddr_in, bytes)> handler);
  static void *process(void *_args);
  void tell(sockaddr_in client_addr, std::string identifier, bytes body);
  bytes promise(sockaddr_in client_addr, std::string identifer, bytes body);
};
};  // namespace prt

#endif