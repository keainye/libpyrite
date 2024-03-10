#ifndef _PRT_LOG
#define _PRT_LOG

namespace prt {
void log(std::string msg);
void warn(std::string msg);
void panic(std::string msg);
}

#endif