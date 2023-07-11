#include "client.h"
#include "log.h"

prt::client::client(const char* Ip, int port, int timeout) {
  WSADATA data;

  if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    panic("WSAStartup error");

  SOCKADDR_IN addrServer;
  addrServer.sin_family = AF_INET;
  addrServer.sin_port = htons(port);
  addrServer.sin_addr.S_un.S_addr = inet_addr(Ip);

  SOCKET socketClient = socket(AF_INET, SOCK_DGRAM, 0);
  if (socketClient == INVALID_SOCKET)
    panic("socket create error");

  this->connection = socketClient;
  this->server = addrServer;
  this->sequence = 0;
  this->timeout = timeout;
}

bool prt::client::add_router(std::string identifier,
                             std::function<bytes(bytes)> controller) {
  if (identifier.find("prt-") == 0)
    return false;

  this->router[identifier] = controller;
  return true;
}

int prt::client::get_sequence() {
  return this->sequence++;
}

prt::client::~client() {
  closesocket(this->connection);
  WSACleanup();
}

prt::bytes prt::client::promise(std::string identifier, prt::bytes body) {
  prt::package req(this->session, identifier, this->get_sequence(), body);
  req.send_to(this->connection);
  this->promise_buffer[req.sequence] = req;
  int time = this->timeout;
  std::mutex ch;
  bool lock = 0;
  std::thread _time([&](int time) {
    Sleep(time);
    ch.lock();
    lock = 1;
    ch.unlock();
  });
  bool getlock;
  prt::bytes respbody;
  while (true) {
    ch.lock();
    getlock = lock;
    ch.unlock();
    this->bufferL[req.sequence]->lock();
    if (this->promise_buffer[req.sequence].identifier == "prt-ack") {
      respbody = this->promise_buffer[req.sequence].body;
      this->promise_buffer.erase(req.sequence);
      this->bufferL[req.sequence]->unlock();
      delete (this->bufferL[req.sequence]);
      this->bufferL.erase(req.sequence);
      break;
    }

    this->bufferL[req.sequence]->unlock();
    if (getlock) {
      delete (this->bufferL[req.sequence]);
      this->bufferL.erase(req.sequence);
      std::cout << "pyrite counterpart timeouted" << std::endl;
      return bytes();
    }
  }

  return respbody;
}

int prt::client::tell(std::string identifier, prt::bytes body) {
  prt::package(this->session, identifier, -1, body).send_to(this->connection);
}

void prt::client::process_ack(prt::package p) {
  if (this->promise_buffer.find(p.sequence) == this->promise_buffer.end()) {
    return;
  }
  this->bufferL[p.sequence]->lock();
  this->promise_buffer[p.sequence] = p;
  this->bufferL[p.sequence]->unlock();
}

void prt::client::process(prt::bytes raw) {
  prt::package req(raw);
  // 无条件相信来自服务器的 session
  this->session = req.session;

  if (req.identifier == "prt-ack") {
    this->process_ack(req);
    return;
  }

  auto find = this->router.find(req.identifier);
  if (find == this->router.end())
    return;
  auto f = (*find).second;
  prt::bytes resp_body = f(req.body);

  // tell 不需要回信
  if (req.sequence < 0)
    return;

  // promise
  prt::package resp(req.session, "prt-ack", req.sequence, resp_body);
  resp.send_to(this->connection);
}

int prt::client::start() {
  if (connect(this->connection, (struct sockaddr*)&this->server,
              sizeof(this->server)) == INVALID_SOCKET)
    panic("connect error");

  char recvBuff[prt::max_transmit_size];
  while (true) {
    int recv_size = recv(this->connection, recvBuff, prt::max_transmit_size, 0);
    if (recv_size < 0) {
      warn("recv error.");
      continue;
    }

    // copy to new raw bytes
    bytes raw(recv_size);
    for (int i = 0; i < recv_size; i++)
      raw[i] = recvBuff[i];

    std::thread p(client::process, this, raw);
    p.detach();
  }
}