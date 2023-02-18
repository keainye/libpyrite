#include "../header/prt_package.h"

PrtPackage::PrtPackage() {
    this->Body="invalid";
    this->Identifier="invalid";
    this->sequence=0;
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

	int sequence=atoi(splits[2]);
	if err != nil {
		return nil, ErrInvalidResponse
	}

	return &PrtPackage{
		Session:    splits[0],
		Identifier: splits[1],
		sequence:   sequence,
		Body:       splits[4],
	}, nil
}

std::string PrtPackage::ToBytes() {
    return format("%s\n%s\n%d\n\n%s",this->Session,this->Identifier,this->sequence,this->Body);
}