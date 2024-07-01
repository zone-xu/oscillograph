    #include "tracerline.h"
//******************************** 示踪线

TracerLine::TracerLine(QCustomPlot *_plot, QObject *parent)
    : QObject(parent),
    plot(_plot)
{
    lineV = nullptr;
    initLine();
}

TracerLine::~TracerLine()
{
    if(plot)
    {
        if(lineV)
            plot->removeItem(lineV);
    }
}

/******************************** 线条初始化 ********************************/
void TracerLine::initLine()
{
    if(plot)
    {
        QPen linesPen(Qt::gray,1,Qt::DashLine); //灰色虚线
        lineV = new QCPItemStraightLine(plot);
        lineV->setLayer("overlay");
        lineV->setPen(linesPen);
        lineV->setSelectable(false);
        lineV->setClipToAxisRect(true); //可剪切到绘图矩形
        lineV->point1->setCoords(0,0);
        lineV->point2->setCoords(0,0);

    }
}

void TracerLine::setVisible(bool vis)
{
    if(lineV)
        lineV->setVisible(vis);
}

/********************************* 更新示踪线的位置 *********************************/
void TracerLine::updatePosition(double xValue)
{
    if(lineV)
        lineV->point1->setCoords(xValue,plot->yAxis->range().lower);    //线的底点
        lineV->point2->setCoords(xValue,plot->yAxis->range().upper);    //线的顶点
}
