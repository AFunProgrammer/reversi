#include "reversiapp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ReversiApp w;
    w.show();
    return a.exec();
}
