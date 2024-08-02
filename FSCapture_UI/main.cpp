#include "FSCapture.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FSCapture w;
    w.show();
    return a.exec();
}
