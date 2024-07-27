#include "mainwidget.h"
#include "secondwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    SecondWidget w2;
    w.show();
    w2.show();
    return a.exec();
}
