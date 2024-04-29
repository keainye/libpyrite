#include "log.h"

#include "string"

#ifdef PRT_DISABLE_LOG

namespace prt {
void log(std::string msg) {}
void warn(std::string msg) {}
void panic(std::string msg) {
  exit(1);
}
}  // namespace prt

#else

namespace prt {
void log(std::string msg) {
  std::printf("[I] %s\n", msg.c_str());
}

void warn(std::string msg) {
  std::printf("[W] %s\n", msg.c_str());
}

void panic(std::string msg) {
  std::printf("[E] %s\n", msg.c_str());
  exit(1);
}
}

#endif
