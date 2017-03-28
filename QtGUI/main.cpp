#include "TestAppQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestAppQt w;
    w.show();
    return a.exec();
}
