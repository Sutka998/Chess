#pragma once
#include <fstream>
class Logging
{
	std::ofstream m_out;
	const std::string m_fileName;
public:
	Logging(void);

	~Logging(void);
};

