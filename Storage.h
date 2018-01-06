#pragma once
#include <vector>

template <class T>
class Storage
{
private:
	std::vector<T> storage;
	unsigned int maxSize;

public:
	Storage(unsigned int localStorageSize = 10);
	virtual T at(unsigned int x) const;
	virtual std::vector<T> getContainer() const;
	bool addMessage(T message);
	void reset();
};

template<class T>
Storage<T>::Storage(unsigned int localStorageSize)
{
	storage.reserve(localStorageSize);
	maxSize = localStorageSize;
}

template<class T>
T Storage<T>::at(unsigned int x) const
{
	if (x < storage.size())
	{
		return storage.at(x);
	}
	else
	{
		return storage.at(0);
	}
}

template<class T>
std::vector<T> Storage<T>::getContainer() const
{
	return storage;
}

template<class T>
bool Storage<T>::addMessage(T message)
{
	storage.emplace_back(message);

	return storage.size() >= maxSize;
}

template<class T>
void Storage<T>::reset()
{
	storage.resize(0);
	storage.reserve(maxSize);
}
