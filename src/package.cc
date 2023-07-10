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
  std::string headers = this->session;
  headers += "\n";
  headers += this->identifier;
  headers += "\n";
  char seqstr[100];
  itoa(this->sequence, seqstr, 10);
  headers += seqstr;
  headers += "\n";
  headers += "\n";
  bytes ret = prt::to_bytes(headers);
  ret = range(ret, 0, ret.size()-1);
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
  this->body = prt::to_bytes(text);
}
std::string prt::package::body_as_string() {
  return to_string(this->body);
}
