#include <fstream>
#include <sstream>
#include <vector>
#include "optionsreader.h"
#include "rapidxml/rapidxml.hpp"

using namespace rapidxml;
using namespace options_reader;

int ConvertCharToInt(const char *v)
{
	std::stringstream ss((std::string(v)));
	int output;
	ss >> output;
	return output;
}

fs::path ConvertCharToPath(const char* v)
{
	fs::path filepath(v);
	return filepath;
}

void OptionsReader::Init()
{
	xml_document<> doc;
	const xml_node<> * root_node;
	std::ifstream file("options.xml");

	std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);

	root_node = doc.first_node("application");

	//window option
	const xml_node<> * application_node = root_node->first_node("windows");

	int x = ConvertCharToInt(application_node->first_attribute("x")->value());
	int y = ConvertCharToInt(application_node->first_attribute("y")->value());
	int width = ConvertCharToInt(application_node->first_attribute("width")->value());
	int height = ConvertCharToInt(application_node->first_attribute("height")->value());

	applicationWindow = std::make_unique<ApplicationWindow>(x,y,width,height);

	//configuration file options
	const xml_node<>* configurationFile_node = root_node->first_node("configurationFile");
	fs::path configurationFilePath = ConvertCharToPath(configurationFile_node->first_attribute("path")->value());

	configurationFile = std::make_unique<ConfigurationFile>(configurationFilePath);
}


