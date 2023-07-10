#include <iostream>
#include <unordered_map>
#include <functional>

#include "prt_package.h"
class server
{
private:
  std::string ip;
  std::unordered_map<std::string, std::function<std::string(std::string)>> router;
  int maxLifeTime;
  int sessionlen;

public:
  server(/* args */);
  ~server();
};
