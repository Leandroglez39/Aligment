#pragma once
#include <string>

class input
{
public:
	static std::string read_file(const std::string& path);

	static std::string* read_file2(const std::string& path, const unsigned size);

};

