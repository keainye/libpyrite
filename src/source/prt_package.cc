#include "../header/prt_package.h"

PrtPackage::PrtPackage()
{
    this->Body = "invalid";
    this->Identifier = "invalid";
    this->sequence = -1;
    this->Session = "invalid";
}

PrtPackage::~PrtPackage() {}

PrtPackage::PrtPackage(std::string Session, std::string Identifier, int sequence, std::string Body)
{
    this->Session = Session;
    this->Body = Body;
    this->sequence = sequence;
    this->Identifier = Identifier;
}

PrtPackage::PrtPackage(std::string raw)
{
    std::vector<std::string> splits = split(raw, "\n");
    if (splits.size() < 5)
    {
        return;
    }
    
    const char *seq = splits[2].c_str();
    int sequence = atoi(seq);

    this->Session = splits[0];
    this->Identifier = splits[1];
    this->sequence = sequence;
    this->Body = splits[3];
}

std::string PrtPackage::ToBytes()
{
    return format("%s\n%s\n%d\n\n%s", this->Session, this->Identifier, this->sequence, this->Body);
}