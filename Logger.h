#pragma once
#include "File.h"
#include "DataBase.h"
#include "IMessage.h"
#include "Preferences.h"
#include "Storage.h"

#include <assert.h>
#include <vector>
#include <map>

template <class T>
class Logger
{
private:
	std::map<std::string, ISaving<T>*> saving;

	Preferences<T> *defaultPref;
	Storage<T*> *storage;

	static Logger *instance;
protected:
	Logger(unsigned int localStorageSize = 50);

public:
	static Logger* getInstance(unsigned int localStorageSize = 50);

	void save() const;
	void save(Preferences<T> *pref) const;

	void message(T *message) const;
	
	void setDefaultPref(Preferences<T> *pref);

	Storage<T*>* getStorage() const;

	void addSavingMethod(std::string name, ISaving<T> *saving);
	ISaving<T>* getSavingByName(std::string name) const;

	template <typename S, typename...Ss>
	void setAutoSavings(bool state, const S &first, const Ss&... rest) const;

private:
	void setAutoSavings(bool state) const {};
};

template <class T>
Logger<T> *Logger<T>::instance = 0;

template<class T>
Logger<T> *Logger<T>::getInstance(unsigned int localStorageSize)
{
	if (instance == 0)
	{
		instance = new Logger<T>(localStorageSize);
	}
	else
	{
		return instance;
	}
}

template<class T>
void Logger<T>::save() const
{
	for (auto const& x : saving)
	{
		if (x.second->getActive())
			x.second->save();
	}

	storage->reset();
}

template<class T>
void Logger<T>::save(Preferences<T>* pref) const
{
	for (auto const& x : saving)
	{
		x.second->save(pref);
	}
}

template<class T>
Logger<T>::Logger(unsigned int localStorageSize)
{
	assert((std::is_base_of<IMessage, T>::value) == true);
	storage = new Storage<T*>(localStorageSize);
	defaultPref = new Preferences<T>(storage);
}

template<class T>
void Logger<T>::message(T * message) const
{
	if (storage->addMessage(message))
	{
		save();
		storage->reset();
	}
}

template<class T>
void Logger<T>::addSavingMethod(std::string name, ISaving<T>* saving) 
{
	if (saving->getPref() == nullptr)
	{
		saving->setPref(defaultPref);
	}
	this->saving.emplace(name, saving);
}

template<class T>
void Logger<T>::setDefaultPref(Preferences<T>* pref)
{
	defaultPref = pref;
}

template<class T>
inline Storage<T*>* Logger<T>::getStorage() const
{
	return storage;
}

template<class T>
ISaving<T>* Logger<T>::getSavingByName(std::string name) const
{
	if (saving.count(name) > 0)
	{
		return saving.find(name)->second;
	}
	else
	{
		return nullptr;
	}
}

template<class T>
template<typename S, typename ...Ss>
void Logger<T>::setAutoSavings(bool state, const S & first, const Ss & ...rest) const
{
	ISaving<T>* temp = getSavingByName(first);

	if (temp != nullptr)
		temp->setActive(state);		

	setAutoSavings(state, rest...);
}
