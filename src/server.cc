#include "server.h"
#include "log.h"

prt::server::server(int _port, i64 _max_lifetime, i64 _timeout)
  : port(_port), max_lifetime(_max_lifetime), timeout(_timeout) {}

void prt::server::start() {
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)
    panic("socket error. cannot start server.");

  sockaddr_in server_addr, client_addr;
  int client_len = sizeof(client_addr);
  memset(&server_addr, 0, sizeof(sockaddr_in));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(this->port);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sockfd, (sockaddr*) &server_addr, sizeof(server_addr)) < 0)
    panic("server binding failed.");
  
  int recv_size;
  char recv_buf[1024];
  while (true) {
    recv_size = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (sockaddr*) &client_addr, &client_len);
    if (recv_size < 0) continue;
    bytes raw(recv_size);
    for (int i = 0; i < recv_size; i++)
      raw[i] = recv_buf[i];
    this->process(client_addr, package(raw));
  }
}

void prt::server::process(sockaddr_in client, package recv) {
  // process tell
  if (this->router.find(recv.identifier) == this->router.end())
    return;
  this->router[recv.identifier](recv.body);
}

bool prt::server::add_router(std::string identifier, std::function<bytes(bytes)> ctrler) {
  if (identifier.find("prt-") >= 0)
    return false;
  this->router[identifier] = ctrler;
}
