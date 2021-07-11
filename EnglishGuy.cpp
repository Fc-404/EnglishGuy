#include "EnglishGuy.h"

#include <io.h>
#include <string>
#include <vector>
#include <fstream>
#include <QTextCodec>
#include <QTime>

EnglishGuy::EnglishGuy(QWidget* parent)
	: QWidget(parent)
	, ini(new Ini())
	, change(NULL)
	, book(NULL)
	, delay(5000)
{
	ui.setupUi(this);

	// hide window bar
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	// this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());

	// backgroup translucent
	this->setAttribute(Qt::WA_TranslucentBackground);

	// set window at topmost
	SetWindowPos((HWND)winId(), HWND_TOPMOST, pos().x(), pos().y(), width(), height(), SWP_SHOWWINDOW);

	// system tray setup
	CreateSystemTrayIcon();

	map<string, string> iniRecord = ini->Get();

	// set forecolor and backcolor depend ini file
	ApplyColor(iniRecord["ForeColor"], iniRecord["BackColor"]);

	// set window position depend ini file	
	this->setGeometry(
		stoi(iniRecord["X"] == "" ? "0" : iniRecord["X"]),
		stoi(iniRecord["Y"] == "" ? "0" : iniRecord["Y"]),
		this->size().width(),
		this->size().height());

	// set delay of word change
	if (iniRecord["Delay"] != "")
	{
		delay = QString::fromStdString(iniRecord["Delay"]).toInt();
	}

	// start words change
	if (iniRecord["wordbook"] != "")
	{
		StartWordsChange(iniRecord["wordbook"]);
	}
}

EnglishGuy::~EnglishGuy() {
	delete ini;
	delete change;
}

void EnglishGuy::enterEvent(QEvent* m)
{
	SetWindowPos((HWND)winId(), HWND_TOPMOST, pos().x(), pos().y(), width(), height(), SWP_SHOWWINDOW);
}

void EnglishGuy::leaveEvent(QEvent* m)
{
}

void EnglishGuy::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mousePos = event->pos();
	}
}

void EnglishGuy::mouseMoveEvent(QMouseEvent* event)
{
	QPoint pos = event->globalPos() - mousePos;
	QDesktopWidget* desktop = QApplication::desktop();
	QPoint* maxPos = new QPoint(
		desktop->screenGeometry().width() - this->width(),
		desktop->screenGeometry().height() - this->height());
	if (pos.x() < 0)
	{
		pos.setX(0);
	}
	if (pos.y() < 0)
	{
		pos.setY(0);
	}
	if (pos.x() > maxPos->x())
	{
		pos.setX(maxPos->x());
	}
	if (pos.y() > maxPos->y())
	{
		pos.setY(maxPos->y());
	}
	this->setGeometry(pos.x(), pos.y(), this->size().width(), this->size().height());
}

void EnglishGuy::mouseReleaseEvent(QMouseEvent* event)
{
	ini->Set("X", to_string(pos().x()));
	ini->Set("Y", to_string(pos().y()));
}

void EnglishGuy::CreateSystemTrayIcon()
{
	QAction* setColor = new QAction(QStringLiteral("SetColor"));
	QAction* showAction = new QAction(QStringLiteral("Show"));
	QAction* exitAction = new QAction(QStringLiteral("Exit"));

	connect(setColor, &QAction::triggered, this, [=]()
		{
			sc = new SetColor();
			connect(sc, &SetColor::SendColor, this, &EnglishGuy::ApplyColor);
			sc->show();
		});
	connect(showAction, &QAction::triggered, this, [=]()
		{ this->show(); });
	connect(exitAction, &QAction::triggered, this, [=]()
		{
			QApplication::exit(0);
		});

	QMenu* wordsMenu = new QMenu(this);
	wordsMenu->setTitle("Select Word Book");
	vector<string> wordsFileName;
	string wordsPath = QCoreApplication::applicationDirPath().toStdString() + "\\Words";
	GetAllFiles(wordsPath, wordsFileName, ".wd");
	for each (string name in wordsFileName)
	{
		QAction* option = new QAction(QString::fromStdString(name));
		connect(option, &QAction::triggered, this, [=]()
			{
				ini->Set("wordbook", name);
				StartWordsChange(name);
			});
		wordsMenu->addAction(option);
	}

	QMenu* trayMenu = new QMenu(this);
	trayMenu->addMenu(wordsMenu);
	trayMenu->addAction(setColor);
	trayMenu->addAction(showAction);
	trayMenu->addAction(exitAction);

	QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(QIcon(":/EnglishGuy/icon.png"));
	trayIcon->setContextMenu(trayMenu);
	trayIcon->show();
}

void EnglishGuy::ApplyColor(string fore, string back)
{
	ui.bg->setStyleSheet(QString("background:%1;").arg(QString::fromStdString(back)));
	ui.English->setStyleSheet(ui.English->styleSheet() + QString("background:%1;").arg(QString::fromStdString(back)));
	ui.Chinese->setStyleSheet(ui.Chinese->styleSheet() + QString("background:%1;").arg(QString::fromStdString(back)));
	ui.English->setStyleSheet(ui.English->styleSheet() + QString("color:%1;").arg(QString::fromStdString(fore)));
	ui.Chinese->setStyleSheet(ui.Chinese->styleSheet() + QString("color:%1;").arg(QString::fromStdString(fore)));
}

void EnglishGuy::GetAllFiles(string path, vector<string>& files, string fileType)
{
	intptr_t hFile = 0;
	struct _finddata_t fileinfo;

	string p;

	if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileinfo)) != -1) {
		do {
			files.push_back(fileinfo.name);
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void EnglishGuy::StartWordsChange(string bookname)
{
	string wordsPath = QCoreApplication::applicationDirPath().toStdString() + "\\Words";
	if (book)
	{
		delete book;
	}
	book = new Ini(wordsPath + '\\' + bookname);
	map<string, string> words = book->Get();

	int wordsL = words.size();
	map<int, string> wordsI;

	int n = 0;
	for each (pair<string, string> var in words)
	{
		wordsI[n++] = var.first;
	}

	if (change)
	{
		change->stop();
		delete change;
	}
	change = new QTimer(this);
	QTime time;
	connect(change, &QTimer::timeout, this, [&, wordsL, words, wordsI] {
		time = QTime::currentTime();
		qsrand(time.msec() + time.second() * 1000);
		int index = qrand() % wordsL;
		QString english = QString::fromStdString(wordsI.at(index));
		QString chinese = QString::fromStdString(words.at(wordsI.at(index)));
		ui.English->setText(english);
		ui.Chinese->setText(chinese);
		});
	change->start(delay);
}

