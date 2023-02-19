#include "define.h"
#include "prt_string.h"
#include <WinSock2.h>
#include <thread>

class PrtPackage
{
public:
   std::string Session;
   std::string Identifier;
   int sequence;
   std::string Body;
   PrtPackage();
   PrtPackage::PrtPackage(std::string raw);
   PrtPackage(std::string Session, std::string Identifier, int sequence, std::string Body);
   ~PrtPackage();
   std::string ToBytes();
};
