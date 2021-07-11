#include "SetColor.h"
#include <winsock.h>

SetColor::SetColor(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	connect(ui.Apply, &QPushButton::clicked, this, &SetColor::Apply);
	connect(ui.Enter, &QPushButton::clicked, this, &SetColor::Enter);
	connect(ui.Cancel, &QPushButton::clicked, this, &SetColor::Cancel);

	ini = new Ini();
	oldColor = ini->Get();
	ui.Forecolor->setText(QString::fromStdString(oldColor["ForeColor"]));
	ui.Backcolor->setText(QString::fromStdString(oldColor["BackColor"]));
}

SetColor::~SetColor()
{
	delete ini;
}

void SetColor::Apply()
{
	QString fore = ui.Forecolor->text();
	QString back = ui.Backcolor->text();
	emit SendColor(fore.toStdString(), back.toStdString());
}

void SetColor::Enter()
{
	Ini ini;
	QString s = QString::fromStdString(ini.Read());
	QString fore = ui.Forecolor->text();
	QString back = ui.Backcolor->text();
	emit SendColor(fore.toStdString(), back.toStdString());
	ini.SetColor(fore.toStdString(), back.toStdString());
	this->close();
}

void SetColor::Cancel()
{
	emit SendColor(oldColor["ForeColor"], oldColor["BackColor"]);
	this->close();
}

QString SetColor::TakeColor()
{
	return QString("%1&%2")
		.arg(ui.Forecolor->text())
		.arg(ui.Backcolor->text());
}
