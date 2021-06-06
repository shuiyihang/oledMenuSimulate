#include "widget.h"

#include <QApplication>

#include "progressring.h"

#include "QTimer"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;


    w.show();
    return a.exec();
}















/**
//    ProgressRing ring(&w);

//    ring.setRange(0,360);
//    ring.setValue(30);
//    ring.resize(200,200);
//    ring.setBgColor(Qt::white);
//    ring.setTextColor(Qt::black);
//    ring.setRingColor(Qt::darkCyan);
//    ring.setRingBgColor(Qt::darkBlue);
//    ring.setRingPadding(20);
//    ring.setRingWidth(30);
//    ring.setCircleColor(Qt::white);
//    ring.setStartAngle(90);
//    ring.setClockWise(true);
//    ring.move(50,80);
//    ring.
*/
