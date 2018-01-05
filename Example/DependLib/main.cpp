#include "dependlib.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DependLib w;
    w.show();
    return a.exec();
}
