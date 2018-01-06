#pragma once
#include <string>

class IMessage
{
public:
	virtual std::string asString() const = 0;
	virtual std::string at(unsigned int x) const = 0;
	virtual unsigned int size() const = 0;
};