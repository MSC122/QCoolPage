#include "mainwindow.h"
#include <QApplication>
#include "screen/mainobject.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainObject mainObject;
    mainObject.setInit();

    return a.exec();
}
