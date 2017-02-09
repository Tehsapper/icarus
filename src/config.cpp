#include "config.hpp"
#include <iostream>
#include <fstream>

Config::Config( const char* filename )
{
	std::cout << "using config file: " << filename << std::endl;
	values = IniParser::parse( filename );
}

const std::string& Config::operator[] ( const std::string& name ) { return get(name); }

const std::string& Config::get ( const std::string& name )
{
	try
	{
		return values.at(name);
	} catch(const std::out_of_range& e)
	{
		throw std::runtime_error(std::string("config: failed to get \"") + std::string(name) + std::string("\" value from config!"));
	}
}

std::string& trim(std::string& str)
{
	static const char* trim_set = " \t\r\n";

	str.erase(0, str.find_first_not_of(trim_set));
	str.erase(str.find_last_not_of(trim_set) + 1);

	return str;
}

value_map_t IniParser::parse( const char* filename )
{
	std::ifstream input(filename);
	std::string line;
	std::string section_name = "";
	value_map_t result;
	size_t line_number = 0;

	if(!input) throw std::runtime_error(std::string("config: failed to open .ini file ") + filename);

	while(std::getline(input, line))
	{
		line_number++;

		if(line.empty()) continue;
		if(line[0] == '#' || line[0] == ';') continue;
		if(line[0] == '[')
		{
			if(line[line.length() - 1] == ']')
			{
				section_name = line.substr(1, line.length() - 2) + ".";
				continue;
			} else throw std::runtime_error(std::string("config: bad section name on line ") + std::to_string(line_number));
		}

		size_t eq_pos = line.find('=');
		if( eq_pos == std::string::npos) throw std::runtime_error(std::string("config: no = symbol in assignment on line ") + std::to_string(line_number));

		std::string name = line.substr(0, eq_pos);
		trim(name);
		name = section_name + name;
		std::string value = line.substr(eq_pos + 1);
		trim(value);

		result[name] = value;
	}

	return result;
}