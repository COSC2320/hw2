#ifndef _INCLUDEtextFile_H
#define _INCLUDEtextFile_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

class Textfile {
public:
	Textfile();
	Textfile(std::string name);
	void setFilename(std::string name);
	void details();
	void readFileContents();
	std::string lineAt(int i);
	~Textfile();
private:
	std::string *lines = NULL;
	std::string filename;
	int numberOfLines = 0;
};

Textfile::Textfile()
{

}

Textfile::Textfile(std::string name)
{
	setFilename(name);
}

void Textfile::setFilename(std::string name)
{
	filename = name;
}

void Textfile::details()
{
	std::cout << "Filename: " << filename << std::endl;
}

void Textfile::readFileContents()
{
	std::ifstream olaf(filename);
	std::string str;
	std::vector<std::string> tokens;

	while (getline(olaf, str))
	{
		if (str.find("#") == 0 || str.length() == 0)
		{
			// Skip this line when it starts with # or the line is empty
		}
		else
		{
			//auto str = "The quick brown fox";
			std::vector<std::string> tokens;

			for (auto i = strtok(&str[0], "\t\n\r"); i != nullptr; i = strtok(nullptr, "\t\n\r"))
				tokens.push_back(i);

			for (int unsigned i = 0; i < tokens.size(); ++i)
			{
				std::cout << tokens[i] << std::endl;
			}
		}
	}
	olaf.close();
}

std::string Textfile::lineAt(int i)
{
	return lines[i];
}

Textfile::~Textfile()
{
	if (NULL != lines)
		delete[] lines;
}

#endif // !_INCLUDEtextFile_H
