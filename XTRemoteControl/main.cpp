#include "XTRemoteControl.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XTRemoteControl w;
    w.show();
    return a.exec();
}
