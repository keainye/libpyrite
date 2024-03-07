#ifndef _PRT_CLIENT_H
#define _PRT_CLIENT_H

#include "functional"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "define.h"
#include "package.h"

namespace prt {
class client {
  int         server_fd;
  sockaddr_in server_addr;
 public:
  connection_state state;
  client(const char* ip, int port, int timeout);
  ~client();
  void start();
  bool add_router(std::string identifier, std::function<bytes(bytes)> handler);
  static void *process(void *recv_pkg);
  int tell(std::string identifier, bytes body);
  bytes promise(std::string identifer, bytes body);
};
}  // namespace prt

#endif