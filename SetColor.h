#pragma once

#include <QWidget>
#include "ui_SetColor.h"
#include <QCloseEvent>
#include <string>
#include "Ini.h"

class SetColor : public QWidget
{
	Q_OBJECT

private:
	Ini *ini;
	map<string, string> oldColor;

public:
	SetColor(QWidget *parent = Q_NULLPTR);
	~SetColor();
private:
	void Apply();
	void Enter();
	void Cancel();
	QString TakeColor();

private:
	Ui::SetColor ui;

signals:
	void SendColor(string, string);
};
