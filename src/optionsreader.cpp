#include <fstream>
#include <sstream>
#include <vector>
#include "optionsreader.h"
#include "rapidxml/rapidxml.hpp"

using namespace rapidxml;
using namespace options_reader;

OptionsReader *OptionsReader::optionsReader = 0;

OptionsReader::OptionsReader()
{
	applicationWindow = 0;

	Init();
}

int ConvertCharToInt(char *v)
{
	std::stringstream ss((std::string(v)));
	int output;
	ss >> output;
	return output;
}

void OptionsReader::Init()
{
	xml_document<> doc;
	xml_node<> * root_node;
	std::ifstream file("options.xml");

	std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);

	root_node = doc.first_node("application");

	xml_node<> * application_node = root_node->first_node("windows");

	int x = ConvertCharToInt(application_node->first_attribute("x")->value());
	int y = ConvertCharToInt(application_node->first_attribute("y")->value());
	int width = ConvertCharToInt(application_node->first_attribute("width")->value());
	int height = ConvertCharToInt(application_node->first_attribute("height")->value());

	applicationWindow = new ApplicationWindow(x,y,width,height);

}

OptionsReader::~OptionsReader()
{
	if (applicationWindow)
		delete applicationWindow;

	applicationWindow = 0;
}



