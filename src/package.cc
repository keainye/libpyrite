#include "package.h"
#include "log.h"

prt::package::package() {
  this->session = "";
  this->identifier = "";
  this->sequence = 0;
  this->body = prt::bytes(0);
}

prt::package::package(prt::bytes raw) {
  this->session = raw.next_string();
  this->identifier = raw.next_string();
  this->sequence = raw.next_int32();
  this->body = raw.range(raw.ptr, raw.size());
}

prt::bytes prt::package::to_bytes() {
  bytes ret(this->session);
  ret = ret + bytes(this->identifier);
  bytes seq(4);
  for (int i = 0; i < 4; i++)
    seq[i] = (moc::byte) (this->sequence >> (i*8));
  ret = ret + seq;
  ret = ret + this->body;
  return ret;
}

bool prt::package::operator==(const prt::package& other) {
  if (other.session != this->session) return false;
  if (other.identifier != this->identifier) return false;
  if (other.sequence != this->sequence) return false;
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
