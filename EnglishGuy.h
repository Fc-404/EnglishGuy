#pragma once

#include <QtWidgets/QWidget>
#include "ui_EnglishGuy.h"
#include "SetColor.h"
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QDesktopWidget>
#include <QString>
#include <QScreen>
#include <QApplication>
#include <Windows.h>
#include "Ini.h"
#include <QTimer>
#include <QDir>

class EnglishGuy : public QWidget
{
    Q_OBJECT

protected:
    QPoint mousePos;

public:
    EnglishGuy(QWidget *parent = Q_NULLPTR);
    ~EnglishGuy();
    enum ColorType { RGB, RGBA, HEX };

protected:
    void enterEvent(QEvent* m);
    void leaveEvent(QEvent* m);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void CreateSystemTrayIcon();
    void GetAllFiles(string, vector<string>&, string);
    void StartWordsChange(string bookname);

private:
    Ui::EnglishGuyClass ui;
    SetColor* sc;
    map<string, string> words;
    Ini* ini, * book;
    QTimer* change;
    int delay;

public slots:
    void ApplyColor(string, string);
};
