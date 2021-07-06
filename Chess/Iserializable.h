#pragma once
#include <fstream>

/**
 * \file ISerializable.h
 * \brief Provides an interface for serialization to/from an input/output file stream.
 */

class ISerializable {
public:
	virtual void Serialize(std::ofstream& os) = 0;
	virtual void Deserialize(std::ifstream& is) = 0;

};