#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <QApplication>

using namespace std;

class Ini
{

public:
	static string execPath;
	string filename;

private:
	fstream ini;
	map<string, string> rules;

public:
	Ini();
	Ini(string);
	~Ini();
	map<string, string> Get();
	void Set(string, string);
	void SetColor(string, string);
	map<string, string> Parsea(string);
	string Form(map<string, string>);
	string Read();
	void Write(string);
	void Merge(map<string, string>*, map<string, string>*);
};
