#include "mainwindow.h"
#include "signin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    signin s1;
    s1.show();

//    w.show();
    return a.exec();
}
