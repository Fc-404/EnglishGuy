#include "Ini.h"

using namespace std;

string Ini::execPath = QCoreApplication::applicationDirPath().toStdString();

Ini::Ini()
	: filename("EnglishGuy.ini")
{
}

Ini::Ini(string filename)
{
	this->filename = filename;
}

Ini::~Ini()
{
}

map<string, string> Ini::Get()
{
	return Parsea(Read());
}

void Ini::Set(string key, string value)
{
	map<string, string> temp, drgo;
	temp.insert(make_pair(key, value));
	drgo = Parsea(Read());
	Merge(&temp, &drgo);
	Write(Form(drgo));
}

void Ini::SetColor(string fore, string back)
{
	map<string, string> color;
	string foreName = "ForeColor";
	string backName = "BackColor";
	color.insert(make_pair(foreName, fore));
	color.insert(make_pair(backName, back));
	map<string, string> drgo = Parsea(Read());
	Merge(&color, &drgo);
	Write(Form(drgo));
}

map<string, string> Ini::Parsea(string str)
{
	map<string, string> pair;
	string line;
	for each (char var in str)
	{
		if (var == '\n' || var == str[str.length() - 1])
		{
			size_t index = line.find('=');
			string key = line.substr(0, index);
			string value = line.substr(index + 1);
			pair.insert(make_pair(key, value));
			line.clear();
			continue;
		}
		line += var;
	}
	return pair;
}

string Ini::Form(map<string, string> pair)
{
	string str;
	map<string, string>::iterator iter;
	iter = pair.begin();
	while (iter != pair.end())
	{
		if (iter->first == "")
		{
			++iter;
			continue;
		}
		str += iter->first + '=' + iter->second + '\n';
		++iter;
	}
	return str;
}

string Ini::Read()
{
	ini.open(filename, ios::in);
	string str;
	if (ini.is_open())
	{
		while (!ini.eof())
		{
			str += ini.get();
		}
		ini.close();
	}

	return str;
}

void Ini::Write(string str)
{
	ini.open(filename, ios::out);
	if (ini.is_open())
	{
		ini << str;
		ini.close();
	}
}

void Ini::Merge(map<string, string>* first, map<string, string>* second)
{
	map<string, string>::iterator f;
	f = first->begin();
	while (f != first->end())
	{
		try
		{
			second->at(f->first);
			(*second)[f->first] = f->second;
		}
		catch (exception m)
		{
			second->insert(make_pair(f->first, f->second));
		}
		++f;
	}
}
