#include "Input.h"
#include <string>
#include <fstream>


using  namespace std;

string input::read_file(const std::string& path)
{
	string line;
	ifstream myfile(path);
	string text;


	if (myfile.is_open())
	{
		while (getline(myfile, line))
			text += line;

		myfile.close();
	}

	return text;
}

string* input::read_file2(const std::string& path, const unsigned size)
{
	string line;
	ifstream myfile(path);
	auto* text = new string[size];
	int i = 0;

	if (myfile.is_open())
	{
		while (getline(myfile, line)) {
			text[i] = line;
			i++;
		}

		myfile.close();
	}

	return text;
}