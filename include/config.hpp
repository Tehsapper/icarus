#pragma once

#ifndef __cplusplus
#  error config.hpp is a C++ header
#endif

#include <map>
#include <string>

typedef std::map<std::string, std::string> value_map_t;

class Config
{
	private:
		value_map_t values;
	public:
		Config(const char* filename);
		const std::string& operator[] (const std::string& name);
		const std::string& get( const std::string& name);
};

std::string& trim(std::string& str);

class IniParser
{
	public:
		static value_map_t parse(const char* filename);
};