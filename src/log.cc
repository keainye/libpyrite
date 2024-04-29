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
  std::printf("\033[32m[I] %s\n\033[0m", msg.c_str());
}

void warn(std::string msg) {
  std::printf("\033[33m[W] %s\n\033[0m", msg.c_str());
}

void panic(std::string msg) {
  std::printf("\033[31m[E] %s\n\033[0m", msg.c_str());
  exit(1);
}
}

#endif
