#ifndef _PRT_LOG
#define _PRT_LOG

#include "iostream"
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
  std::printf("[prt info] %s\n", msg);
}

void warn(std::string msg) {
  std::printf("[prt warn] %s\n", msg);
}

void panic(std::string msg) {
  std::printf("[prt error!] %s\n", msg);
  exit(1);
}
}  // namespace prt

#endif
#endif