#include "client.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "pthread.h"

#include "log.h"

prt::client::client(const char* ip, int port) {
  this->state = prt::closed;
  if ((this->server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    char warn_msg[100];
    sprintf(warn_msg, "Pyrite connect creation failed. Server addr: %s:%d", ip, port);
    prt::warn(warn_msg);
    return;
  }

  memset(&this->server_addr, 0, sizeof(this->server_addr));
  this->server_addr.sin_family = AF_INET;
  this->server_addr.sin_port = htons(port);
  this->server_addr.sin_addr.s_addr = inet_addr(ip);

  this->sequence = 0;
  this->state = prt::established;
}

prt::client::~client() {
  close(this->server_fd);
}

void prt::client::start() {
  int recv_len;
  socklen_t l;
  pthread_t tid;
  char buf[prt::max_transmit_size];

  while (true) {
    recv_len = recvfrom(this->server_fd, buf, prt::max_transmit_size, MSG_CONFIRM, (struct sockaddr *) &this->server_addr, &l);
    process_args *args = new process_args;
    args->ptr = this;
    args->pkg = prt::package(prt::bytes(buf, recv_len));
    pthread_create(&tid, NULL, this->process, (void *) args);
  }
}

bool prt::client::set_handler(std::string identifier, std::function<bytes(bytes)> handler) {
  if (identifier.find("prt-") == 0)
    return false;
  this->router[identifier] = handler;
  return true;
}

void *prt::client::process(void *_args) {
  assert(_args);
  process_args *args = (process_args *) _args;
  prt::client *client_ptr = (prt::client *) args->ptr;
  prt::package recv_pkg = args->pkg;
  delete args;
  
  if (!client_ptr->router.count(recv_pkg.identifier))
    return nullptr;

  prt::bytes reply = client_ptr->router[recv_pkg.identifier](recv_pkg.body);
  if (reply.size())
    recv_pkg.send_to(client_ptr->server_fd, client_ptr->server_addr);
  return nullptr;
}

void prt::client::tell(std::string identifier, bytes body) {
  prt::package pkg(-1, identifier, body);
  pkg.send_to(this->server_fd, this->server_addr);
}
