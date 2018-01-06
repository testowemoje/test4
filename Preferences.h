#pragma once
#include <vector>
#include "Storage.h"

template <class T>
class Preferences
{
protected:
	Storage<T*> *storage;

public:
	Preferences(Storage<T*> *_storage);
	virtual std::vector<T*> getMessages();
};

template<class T>
Preferences<T>::Preferences(Storage<T*>* _storage)
{
	storage = _storage;
}

template<class T>
std::vector<T*> Preferences<T>::getMessages()
{
	std::vector<T*> returnCon;

	for (T* obj : storage->getContainer())
	{
		returnCon.emplace_back(new T(*obj));
	}

	return returnCon;
}