#ifndef TRACERPLOT_H
#define TRACERPLOT_H

#include <QObject>
#include "qcustomplot.h"

//***************************** 游标

class TracerPlot : public QObject
{
    Q_OBJECT
public:
    TracerPlot(QCustomPlot *_plot,QObject *parent = nullptr);
    ~TracerPlot();
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setLabelPen(const QPen &pen);
    void setText(const QString &text);
    void setVisible(bool vis);
    void updatePosition(double xValue, double yValue);  //更新上方游标跟下方标签框的位置

protected:
    QCustomPlot *plot;  //绘制区域
    QCPItemTracer *tracer;  //游标
    QCPItemText *label;     //标签框：显示坐标位置
    bool visible;   //游标是否可视。true:可视

};

#endif // TRACERPLOT_H

