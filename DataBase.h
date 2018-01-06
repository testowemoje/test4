#pragma once
#include "ISaving.h"
#include <SQLAPI.h>

template <class T>
class DataBase : public ISaving<T>
{
private:
	std::string server;
	std::string database;
	std::string user;
	std::string password;
	std::string table;

	eSAClient DB_Type;
	SAConnection connection;

public:
	DataBase(std::string &_server, std::string &_database, std::string &_user, std::string &_password, eSAClient _DB_Type = eSAClient::SA_MySQL_Client,std::string &_table = std::string("Logs"));

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
DataBase<T>::DataBase(std::string & _server, std::string & _database, std::string & _user, std::string & _password, eSAClient _DB_Type, std::string & _table)
	:
	server(_server),
	database(_database),
	user(_user),
	password(_password),
	DB_Type(_DB_Type),
	table(_table)
{}

template<class T>
void DataBase<T>::save()
{
	save(this->pref);
}

template<class T>
void DataBase<T>::save(Preferences<T>* pref)
{
	try
	{
		if (!connection.isConnected())
		{
			connection.Connect((server + "@" + database).c_str(), user.c_str(), password.c_str(), DB_Type);
		}

		if(connection.isConnected())
		{
			if (t1.joinable())
			{
				t1.join();
			}

			t1 = std::thread(&DataBase::_save, this, pref->getMessages());
		}
	}
	catch (SAException &e)
	{
		printf("%s\n", (const char*)e.ErrText());
	}
}

template<class T>
void DataBase<T>::setPref(Preferences<T>* pref)
{
	this->pref = pref;
}

template<class T>
Preferences<T>* DataBase<T>::getPref() const
{
	return pref;
}

template<class T>
bool DataBase<T>::getActive()
{
	return isActive;
}

template<class T>
void DataBase<T>::setActive(bool x)
{
	isActive = x;
}

template<class T>
void DataBase<T>::_save(std::vector<T*> container)
{
	auto it = container.begin();

	std::string *commandStr;

	while (it != container.end())
	{
		commandStr = new std::string("INSERT INTO `logs` VALUES ");

		for (int i = 0; i < 100 && it != container.end(); i++, ++it)
		{
			Message *temp = *it;

			*commandStr += [&temp]
			{
				std::string returnVal = "(NULL,";
				for (unsigned int i = 0; i < temp->size(); i++)
				{
					returnVal += "'" + temp->at(i) + "'";
					if (i != temp->size() - 1)	returnVal += ",";
				}
				returnVal += ")";
				return returnVal;
			}();

			if (i != 99 && it + 1 != container.end())
				*commandStr += ",";
		}
		commandStr->append(";");

		std::cout << *commandStr;

		SACommand command(&connection, (*commandStr).c_str());
		command.Execute();

		delete(commandStr);
	}
}
