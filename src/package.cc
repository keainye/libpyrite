#include "package.h"
#include "log.h"

prt::package::package() {}

bool prt::bytes_to_package(package& pac, bytes raw) {
  int cntln = 0, idx[4], ptr;
  for (int i = 0; i < raw.size(); i++) {
    if (raw[i] != '\n') continue;
    cntln++;
    if (cntln >= 5) break;
    idx[cntln-1] = i;
  }
  if (cntln < 4) return false;
  char session[idx[0]+1], identifier[idx[1]-idx[0]], seq_str[idx[2]-idx[1]];
  // cast "session"
  ptr = 0;
  for (int i = 0; i < idx[0]; i++)
    session[ptr++] = raw[i];
  session[ptr] = 0;
  // cast "identifier"
  ptr = 0;
  for (int i = idx[0]+1; i < idx[1]; i++)
    identifier[ptr++] = raw[i];
  identifier[ptr] = 0;
  // cast "sequence"
  ptr = 0;
  for (int i = idx[1]+1; i < idx[2]; i++)
    seq_str[ptr++] = raw[i];
  seq_str[ptr] = 0;
  int seq = atoi(seq_str);
  // end cast
  pac.session = session;
  pac.identifier = identifier;
  pac.sequence = seq;
  pac.body = prt::range(raw, idx[3]+1, raw.size());
}