#pragma once
#include <vector>
#include "IMessage.h"

class Message : public IMessage
{
protected:
	std::vector<std::string> fields;

public:
	std::string asString() const override;
	std::string at(unsigned int x) const override;
	unsigned int size() const override;
	Message(std::string lvl, std::string test1, std::string test2);
};