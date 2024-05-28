#include "client.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include "pthread.h"

#include <mocutils/log.h>

prt::client::client(const char* ip, int port) {
  #ifdef Windows
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data))
      moc::panic("WSAStartup failed.");
  #endif
  this->state = prt::closed;
  if ((this->server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    moc::warnf("Pyrite connect creation failed. Server addr: %s:%d.", ip, port);
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
  #ifdef Windows
    WSACleanup();
  #endif
}

void prt::client::start() {
  int recv_len;
  socklen_t l = sizeof(this->server_addr);
  pthread_t tid;
  char buf[prt::max_transmit_size];

  while (true) {
    recv_len = recvfrom(this->server_fd, buf, prt::max_transmit_size, 0, (struct sockaddr *) &this->server_addr, &l);
    if (recv_len < 0) {
      static int counter;
      counter %= 32;
      if (++counter) continue;
      moc::warnf("invalid recv_len: %d.", recv_len);
      continue;
    }
    process_args *args = new process_args;
    args->ptr = this;
    args->pkg = prt::package(prt::bytes(buf, recv_len));
    pthread_create(&tid, NULL, this->process, (void *) args);
  }
}

namespace prt {
void *client_async_runner(void *args) {
  client *c = (client *) args;
  c->start();
  return nullptr;
}
};

void prt::client::async() {
  pthread_t tid;
  pthread_create(&tid, NULL, client_async_runner, (void *) this);
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

  // process prt ack
  if (recv_pkg.identifier == "prt-ack") {
    if (client_ptr->promise_buf[recv_pkg.sequence])
      *client_ptr->promise_buf[recv_pkg.sequence] < recv_pkg;
    return nullptr;
  }
  
  if (!client_ptr->router.count(recv_pkg.identifier))
    recv_pkg.identifier = "*";

  if (!client_ptr->router.count(recv_pkg.identifier))
    return nullptr;

  prt::bytes reply = client_ptr->router[recv_pkg.identifier](recv_pkg.body);
  if (!reply.size())
    return nullptr;
  prt::package reply_pkg(recv_pkg.sequence, "prt-ack", recv_pkg.body);
  reply_pkg.send_to(client_ptr->server_fd, client_ptr->server_addr);
  return nullptr;
}

void prt::client::tell(std::string identifier, bytes body) {
  prt::package pkg(-1, identifier, body);
  pkg.send_to(this->server_fd, this->server_addr);
}

prt::bytes prt::client::promise(std::string identifer, bytes body) {
  int seq = this->sequence++;
  prt::package pkg(seq, identifer, body);
  pkg.send_to(this->server_fd, this->server_addr);
  this->promise_buf[seq] = new moc::channel<prt::package>;
  prt::package reply;
  *this->promise_buf[seq] > reply;
  delete this->promise_buf[seq];
  return reply.body;
}
