#pragma once
#include <fstream>

class ISerializable {
public:
	virtual void Serialize(std::ofstream& os) = 0;
	virtual void Deserialize(std::ifstream& is) = 0;

};