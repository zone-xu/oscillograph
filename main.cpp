#include "slidingwindow.h"
#include <QApplication>
#include <QtCore>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

    QTranslator s;
    s.load(":/trans_osci.qm");
    a.installTranslator(&s);

    SlidingWindow slidingwindow;

    slidingwindow.show();

    return a.exec();
}
