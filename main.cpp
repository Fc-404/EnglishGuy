#include "EnglishGuy.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EnglishGuy w;
    w.show();
    return a.exec();
}
