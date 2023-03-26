
#include "Exception.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>


static std::string joins(std::vector<std::string> *ss, std::string seprator)
{
	int len = ss->size();
	if (len == 0)
	{
		return "";
	}
	if (len == 1)
	{
		return ss->at(0);
	}

	std::string ok = "";
	for (int i = 0; i < len; i++)
	{
		ok += seprator;
		ok += ss->at(i);
		ok += "\n";
	}

	return ok;
}

Exception::Exception(std::string message, const char *function, const char *file, int line)
{
	this->message = message;
	this->function = function;
	this->file = file;
	this->line = line;
}

std::string Exception::getMessage()
{
	return this->message;
}

std::string Exception::getStackTrace()
{
	std::string stackTrace = function;
	stackTrace += "(";
	stackTrace += file;
	stackTrace += ":";
	stackTrace += std::to_string(line);
	stackTrace += ") ";
	stackTrace += " : ";
	stackTrace += message;
	traces.insert(traces.begin(), stackTrace);

	return joins(&traces, "  -- ");
}

void Exception::traceMessage(std::string message, const char *function, const char *file, int line)
{
	std::string stackTrace = function;

	stackTrace += "(";
	stackTrace += file;
	stackTrace += ":";
	stackTrace += std::to_string(line);
	stackTrace += ") ";
	stackTrace += " : ";
	stackTrace += message;

	traces.push_back(stackTrace);
}

void Exception::trace(const char *function, const char *file, int line)
{
	std::string stackTrace = function;

	stackTrace += "(";
	stackTrace += file;
	stackTrace += ":";
	stackTrace += std::to_string(line);
	stackTrace += ") ";

	traces.push_back(stackTrace);
}