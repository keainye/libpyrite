#include "server.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#include "log.h"

prt::server::server(int port) {
  this->state = closed;
  if ((this->server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    prt::panic("Pyrite bind port failed.");

  memset(&this->server_addr, 0, sizeof(struct sockaddr_in));
  this->server_addr.sin_family = AF_INET;
  this->server_addr.sin_port = htons(port);
  this->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(this->server_fd, (struct sockaddr *) &this->server_addr, sizeof(this->server_addr)) < 0)
    prt::panic("Pyrite server binding failed.");

  this->sequence = 0;
  this->state = prt::established;
}

void prt::server::start() {
  int recv_len;
  socklen_t l;
  sockaddr_in client_addr;
  pthread_t tid;
  char buf[prt::max_transmit_size];

  while (true) {
    recv_len = recvfrom(this->server_fd, buf, prt::max_transmit_size, 0, (struct sockaddr *) &client_addr, &l);
    ptr_package *args = new ptr_package {this, client_addr, prt::package(prt::bytes(buf, recv_len))};
    pthread_create(&tid, NULL, this->process, (void *) args);
  }
}

bool prt::server::set_handler(std::string identifier, std::function<bytes(sockaddr_in, bytes)> handler) {
  if (identifier.find("prt-") == 0)
    return false;
  this->router[identifier] = handler;
  return true;
}

void *prt::server::process(void *_args) {
  assert(_args);
  ptr_package *args = (ptr_package *) _args;
  prt::server *server_ptr = (prt::server *) args->ptr;
  prt::package recv_pkg = args->pkg;
  sockaddr_in client_addr = args->addr;
  delete args;
  
  if (!server_ptr->router.count(recv_pkg.identifier))
    return nullptr;

  prt::bytes reply = server_ptr->router[recv_pkg.identifier](client_addr, recv_pkg.body);
  if (reply.size())
    recv_pkg.send_to(server_ptr->server_fd, client_addr);
  return nullptr;
}

void prt::server::tell(sockaddr_in client_addr, std::string identifier, bytes body) {
  prt::package pkg(-1, identifier, body);
  pkg.send_to(this->server_fd, client_addr);
}
