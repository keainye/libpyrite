#ifndef _PRT_SERVER_H
#define _PRT_SERVER_H

#include "define.h"
#include "map"
#include "mocutils/channel.h"
#include "package.h"
#include "winsock2.h"

namespace prt {
class client_data {
  sockaddr_in addr;
  i64 last_acpt;
  i32 seq;
  std::map<i32, moc::channel<package>> promises;
};
};  // namespace prt

#endif