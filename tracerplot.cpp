#include "tracerplot.h"

//***************************** 游标

TracerPlot::TracerPlot(QCustomPlot *_plot, QObject *parent)
    : QObject(parent)
    , plot(_plot)
{
    tracer = Q_NULLPTR;
    label = Q_NULLPTR;
    if(plot)
    {
        QColor clrDefault(Qt::blue);
        QBrush brushDefault(Qt::SolidPattern);
        QPen penDefault(clrDefault);
        penDefault.setWidthF(0.5);

        tracer = new QCPItemTracer(plot);
        tracer->setStyle(QCPItemTracer::tsCircle);  //光标形状o
        tracer->setPen(penDefault);
        tracer->setBrush(brushDefault);
        tracer->setSelectable(false);
        tracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
        tracer->position->setTypeY(QCPItemPosition::ptPlotCoords);
        tracer->setSize(10);

        label = new QCPItemText(plot);
        label->setLayer("overlay"); //置于最上层
        label->setClipToAxisRect(false);
        label->setPen(QPen(Qt::gray));
        label->setBrush(Qt::SolidPattern);
        label->setPadding(QMargins(5,5,5,5));
        label->position->setTypeX(QCPItemPosition::ptPlotCoords);
        label->position->setTypeY(QCPItemPosition::ptAxisRectRatio);
        label->position->setParentAnchorX(tracer->position);

        label->setFont(QFont("Arial",8));
        label->setColor(Qt::black);
        label->setText("null");
        label->setSelectable(false);
        label->setPositionAlignment(Qt::AlignRight  | Qt::AlignHCenter);
    }
}

TracerPlot::~TracerPlot()
{
    if(plot)
    {
        if(tracer)
            plot->removeItem(tracer);
        if(label)
            plot->removeItem(label);
    }
}

void TracerPlot::setPen(const QPen &pen)
{
    if(tracer)
        tracer->setPen(pen);
}

void TracerPlot::setBrush(const QBrush &brush)
{
    if(tracer)
        tracer->setBrush(brush);
}

void TracerPlot::setLabelPen(const QPen &pen)
{
    if(label)
    {
       label->setPen(pen);
       label->setBrush(Qt::NoBrush);
       label->setColor(pen.color());
    }
}

void TracerPlot::setText(const QString &text)
{
    if(label)
        label->setText(text);
}

void TracerPlot::setVisible(bool vis)
{
    visible = vis;
    if(label)
        label->setVisible(visible);
    if(tracer)
        tracer->setVisible(visible);
}

/******************************* 更新上方游标跟下方标签框的位置 *******************************/
void TracerPlot::updatePosition(double xValue, double yValue)
{
    if(!visible)
    {
        setVisible(true);
    }
    if(xValue > plot->xAxis->range().upper) //如果超过x轴最大值，那就等于最大值
    {
        xValue = plot->xAxis->range().upper;
    }
    else if(xValue < plot ->xAxis->range().lower)   //如果小于x轴最小值，那就等于最小值
    {
        xValue = plot->xAxis->range().lower;
    }
    else if(yValue < plot->yAxis->range().lower)
    {
        yValue = plot->yAxis->range().lower;
    }
    else if(yValue > plot->yAxis->range().upper)
    {
        yValue = plot->yAxis->range().upper;
    }

    tracer->position->setCoords(xValue,yValue); //更新游标坐标位置
    label->setText(QString("X:%1,Y:%2").arg(xValue).arg(yValue));   //设置标签框的文本:坐标位置
}
