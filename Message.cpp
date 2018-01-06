#include "Message.h"

std::string Message::asString() const
{
	std::string temp = "";

	for each (std::string field in fields)
	{
		temp += field + " | ";
	}

	temp.resize(temp.size() - 3);

	return temp;
}

std::string Message::at(unsigned int x) const
{
	return fields.at(x);
}

unsigned int Message::size() const
{
	return fields.size();
}

Message::Message(std::string lvl, std::string test1, std::string test2)
{
	fields.emplace_back(lvl);
	fields.emplace_back(test1);
	fields.emplace_back(test2);
}
