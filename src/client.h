#ifndef _PRT_CLIENT_H
#define _PRT_CLIENT_H

#include "functional"
#include "map"
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
  std::map<std::string, std::function<bytes(bytes)>> router;

  int sequence;
 public:
  connection_state state;
  client(const char* ip, int port);
  ~client();
  void start();
  bool set_handler(std::string identifier, std::function<bytes(bytes)> handler);
  static void *process(void *_args);
  void tell(std::string identifier, bytes body);
  bytes promise(std::string identifer, bytes body);
};
}  // namespace prt

#endif