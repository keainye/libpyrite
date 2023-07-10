#include <functional>
#include <unordered_map>

#include "prt_package.h"

class client {
 private:
  SOCKADDR_IN server;
  std::unordered_map<std::string, std::function<std::string(std::string)>*>
      router;
  std::string session;
  int sequence;
  int timeout;
  std::unordered_map<int, std::mutex*> bufferL;
  std::unordered_map<int, PrtPackage> promiseBuffer;
  SOCKET connection;
  void process(std::string raw);
  void processAck(PrtPackage p);
  int getSequence();

 public:
  client(const char* Ip, int port, int timeout);
  ~client();
  int Start();
  int Tell(std::string identifier, std::string body);
  bool AddRouter(std::string identifier,
                 std::function<std::string(std::string)>* controller);
  std::string Promise(std::string identifer, std::string body);
};
