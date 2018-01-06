#pragma once
#include "Preferences.h"

template <class T>
class ISaving
{
protected:
	Preferences<T> *pref;
	bool isActive;

public:
	virtual void save() {};
	virtual void save(Preferences<T> *pref) = 0;

	virtual void setPref(Preferences<T> *pref) = 0;
	virtual Preferences<T>* getPref() const = 0;

	virtual bool getActive() = 0;
	virtual void setActive(bool x) = 0;
};