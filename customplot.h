#ifndef CUSTOMPLOT_H
#define CUSTOMPLOT_H

#include <QKeyEvent>
#include <QObject>
#include "qcustomplot.h"

class TracerPlot;
class TracerLine;

//***************************** 绘制区域

class CustomPlot : public QCustomPlot
{
    Q_OBJECT
public:
    CustomPlot(QWidget *parent = nullptr);
    ~CustomPlot();
    void showTracer(bool show); //显示示踪线

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual bool event(QEvent *event);

private:
    TracerLine *lineTracer; //示踪线
    QList<TracerPlot *> dataTracers;    //所有曲线上的游标
    bool m_isShowTracer;    //是否显示示踪线。true:显示

signals:
    void signal_start();
};

#endif // CUSTOMPLOT_H
