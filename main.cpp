#include "widget.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(new MyStyle);
    Widget w;
    w.show();

    return a.exec();
}
