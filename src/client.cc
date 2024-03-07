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

prt::client::client(const char* ip, int port, int timeout) {
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
  this->server_addr.sin_addr.s_addr = INADDR_ANY;

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
    ptr_package *args = new ptr_package {this, ptr::package(prt::bytes(buf, recv_len))};
    pthread_create(&tid, NULL, this->process, (void *) args);
  }
}

void *prt::client::process(void *_args) {
  assert(args);
  ptr_package *args = (ptr_package *) _args;
  prt::client *client_ptr = args->ptr;
  prt::package recv_pkg = args->package;
  delete args;
  
  if (!this->router.count(recv_pkg.identifier))
    return nullptr;

  prt::bytes reply = this->router[recv_pkg.identifier](this->body);
  if (!reply.size())
    return nullptr;

  recv_pkg.send_to(client_ptr->server_fd, client_ptr->server_addr);
  return nullptr;
}

void prt::client::tell(std::string identifier, bytes body) {
  prt::package pkg(-1, identifier, body);
  pkg.send_to(this->server_fd, this->server_addr);
}
