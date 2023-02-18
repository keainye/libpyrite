#include "../header/prt_package.h"

PrtPackage::PrtPackage() {
    this->Body="invalid";
    this->Identifier="invalid";
    this->sequence=-1;
    this->Session="invalid";
}

PrtPackage::~PrtPackage() {}

PrtPackage::PrtPackage(std::string Session, std::string Identifier, int sequence,std::string Body) {
    this->Session = Session;
    this->Body = Body;
    this->Identifier = Identifier;
}

PrtPackage PrtPackage::CastToPrtPackage(std::string raw) {
	std::vector<std::string>splits = split(raw,"\n");
	if (splits.size() < 5) {
		return PrtPackage();
	}
    const char *seq = splits[2].c_str();
	int sequence=atoi(seq);

	return PrtPackage(splits[0],splits[1],sequence,splits[4]);
}

std::string PrtPackage::ToBytes() {
    return format("%s\n%s\n%d\n\n%s",this->Session,this->Identifier,this->sequence,this->Body);
}