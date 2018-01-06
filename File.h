#pragma once
#include "ISaving.h"
#include <fstream>
#include <thread>

template <class T>
class file : public ISaving<T>
{
private:
	std::string filePath;
	std::string fileName;

public:
	file(std::string path, std::string name);

	virtual void save() override;
	virtual void save(Preferences<T> *pref) override;

	virtual void setPref(Preferences<T> *pref) override;
	virtual Preferences<T>* getPref() const override;

	virtual bool getActive() override;
	virtual void setActive(bool x) override;

protected:
	std::thread t1;
	void _save(std::vector<T*> container);
};

template<class T>
file<T>::file(std::string path, std::string name)
{
	filePath = path;
	fileName = name;
	this->pref = nullptr;
}

template<class T>
void file<T>::save()
{
	save(this->pref);
}

template<class T>
void file<T>::save(Preferences<T>* pref)
{
	if (t1.joinable())
	{
		t1.join();
	}

	t1 = std::thread(&file::_save,this,pref->getMessages());
}

template<class T>
void file<T>::setPref(Preferences<T>* pref)
{
	this->pref = pref;
}

template<class T>
Preferences<T>* file<T>::getPref() const
{
	return pref;
}

template<class T>
bool file<T>::getActive()
{
	return isActive;
}

template<class T>
void file<T>::setActive(bool x)
{
	isActive = x;
}

template<class T>
void file<T>::_save(std::vector<T*> conteiner)
{
	std::ofstream outfile;
	outfile.open(filePath + fileName, std::fstream::app);

	for each (T* message in conteiner)
	{
		outfile << message->asString() << "\n";
	}

	outfile << "\n";
	outfile.close();
}
