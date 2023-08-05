// #ifndef _PRT_CLIENT_H
// #define _PRT_CLIENT_H

// #include <functional>
// #include <unordered_map>
// #include "mutex"
// #include "thread"
// #include "winsock2.h"

// #include "define.h"
// #include "package.h"

// namespace prt {
// class client {
//  private:
//   SOCKADDR_IN server;
//   std::unordered_map<std::string, std::function<bytes(bytes)>> router;
//   std::string session;
//   int sequence;
//   int timeout;
//   std::unordered_map<int, std::mutex*> bufferL;
//   std::unordered_map<int, prt::package> promise_buffer;
//   SOCKET connection;
//   void process(bytes raw);
//   void process_ack(prt::package p);
//   int get_sequence();

//  public:
//   client(const char* Ip, int port, int timeout);
//   ~client();
//   int start();
//   int tell(std::string identifier, bytes body);
//   bool add_router(std::string identifier,
//                  std::function<bytes(bytes)> controller);
//   prt::bytes promise(std::string identifer, bytes body);
// };
// }  // namespace prt

// #endif