#ifndef TRACERLINE_H
#define TRACERLINE_H

#include <QObject>
#include "qcustomplot.h"

class TracerLine : public QObject
{
    Q_OBJECT
public:
    TracerLine(QCustomPlot *_plot, QObject *parent = nullptr);
    ~TracerLine();
    void initLine();    //线条初始化
    void setVisible(bool vis);
    void updatePosition(double xValue); //更新示踪线的位置

protected:
    QCustomPlot *plot;  //绘制区域
    QCPItemStraightLine *lineV; //示踪线:直线

};

#endif // TRACERLINE_H
