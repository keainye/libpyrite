#ifndef _PRT_DEFINE_H
#define _PRT_DEFINE_H

#include "mocutils/byte.h"

typedef long i32;
typedef long long i64;

#define assert(expr) if (!expr) exit(1);

namespace prt {
  const int max_transmit_size = 1024;
  typedef moc::byte byte;
  typedef moc::bytes bytes;
  enum connection_state {
    closed,
    established,
  };
}

#endif