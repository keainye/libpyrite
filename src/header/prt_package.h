#include"prt_string.h"
class PrtPackage
{
public:
   std::string Session;
   std::string Identifier;
   int sequence;
   std::string Body;
   PrtPackage();

   PrtPackage(std::string Session, std::string Identifier, int sequence,std::string Body);
   ~PrtPackage();
   PrtPackage CastToPrtPackage(std::string raw);
   std::string ToBytes();
};

