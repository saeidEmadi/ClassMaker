#include "Widget.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication::setStyle("Fusion");

    QApplication a(argc, argv);

    Widget w;
    w.show();

    return a.exec();
}
