#pragma once
#include <string>
#include <vector>

using namespace std;

class input
{
public:
	static std::string read_file(const std::string& path);

	static std::string* read_file2(const std::string& path, const unsigned size);

	static  vector<string> read_sequence(string path);


};

