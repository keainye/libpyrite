#include "package.h"
#include "log.h"

prt::package::package() {
  this->sequence = 0;
  this->identifier = "";
  this->body = prt::bytes(0);
}

prt::package::package(prt::bytes raw) {
  this->sequence = raw.next_int32();
  this->identifier = raw.next_string();
  this->body = raw.range(raw.ptr, raw.size());
}

prt::package::package(i32 sequence, std::string identifier, bytes body) {
  this->sequence = sequence;
  this->identifier = identifier;
  this->body = body;
}

prt::bytes prt::package::to_bytes() {
  bytes ret(4);
  for (int i = 0; i < 4; i++)
    ret[i] = (moc::byte) (this->sequence >> (i*8));
  ret = ret + bytes(this->identifier);
  return ret + this->body;
}

bool prt::package::operator==(const prt::package& other) {
  if (other.sequence != this->sequence) return false;
  if (other.identifier != this->identifier) return false;
  if (other.body != this->body) return false;
  return true;
}

bool prt::package::operator!=(const prt::package& other) {
  return !(this->operator==(other));
}

void prt::package::set_body(std::string text) {
  this->body = bytes(text);
}

std::string prt::package::body_as_string() {
  return this->body.to_string();
}

void prt::package::send_to(SOCKET connection) {
  bytes pkg_bytes = this->to_bytes();
  if (pkg_bytes.size() > prt::max_transmit_size)
    prt::panic("content overflowed");
  
  char msg[pkg_bytes.size()];
  for (int i = 0; i < pkg_bytes.size(); i++)
    msg[i] = pkg_bytes[i];
  // TODO: find API
  // send(connection, msg, pkg_bytes.size(), 0);
}
