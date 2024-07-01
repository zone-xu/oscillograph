#include "tracerplot.h"
#include "tracerline.h"
#include "customplot.h"

//***************************** 绘制区域

CustomPlot::CustomPlot(QWidget *parent)
    : QCustomPlot (parent)
    , lineTracer(Q_NULLPTR)
    , dataTracers(QList<TracerPlot *>())
    , m_isShowTracer(false)
{

}

CustomPlot::~CustomPlot()
{
    if(lineTracer != Q_NULLPTR) {
        delete lineTracer;
    }
}

/***************************** 显示示踪线 *****************************/
void CustomPlot::showTracer(bool show)
{
    m_isShowTracer = show;
    if(lineTracer)
        lineTracer->setVisible(m_isShowTracer);
}

void CustomPlot::mouseMoveEvent(QMouseEvent *event)
{
    QCustomPlot::mouseMoveEvent(event);
    if(m_isShowTracer)
    {
        int x_pos = event->pos().x();

        double x_val = this->xAxis->pixelToCoord(x_pos);    //将鼠标移动坐标转为x轴坐标

        if(lineTracer == Q_NULLPTR)
            lineTracer = new TracerLine(this);
        lineTracer->updatePosition(x_val);  //更新示踪线位置

        int nTracerCount = dataTracers.count(); //获取游标数
        int nGraphCount = graphCount(); //获取数据曲线条数
        if(nTracerCount < nGraphCount)  //若设置有新的曲线
        {
            for(int i = nTracerCount; i < nGraphCount; ++i)
            {
                TracerPlot *tracer = new TracerPlot(this);  //补齐所有曲线上的游标（一条曲线，一个游标）
                dataTracers.append(tracer);
            }
        }
        else if(nTracerCount > nGraphCount) //若取消了之前设置的曲线
        {
            for(int i = nGraphCount; i < nTracerCount; ++i)
            {
                TracerPlot *tracer = dataTracers[i];
                if(tracer)
                {
                    tracer->setVisible(false);  //把冗余的游标设置不可见
                }
            }
        }
        for(int i = 0; i < nGraphCount; ++i)    //对于曲线上的游标进行设置
        {
            TracerPlot *tracer = dataTracers[i];
            if(!tracer)
                tracer = new TracerPlot(this);
            tracer->setVisible(true);
            tracer->setPen(this->graph(i)->pen());  //设置游标线形、颜色等
            tracer->setBrush(Qt::NoBrush);
            tracer->setLabelPen(this->graph(i)->pen()); //设置游标标签框线形、颜色等
            auto iter = this->graph(i)->data()->findBegin(x_val);   //从左查找
            double value = iter->mainValue();   //获取纵坐标数值
            tracer->updatePosition(x_val,value);    //更新示踪点位置
        }

        this->replot();
    }
}

bool CustomPlot::event(QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *ev = static_cast<QKeyEvent*>(event);
        if(ev->key() == Qt::Key_Space)
        {
            emit signal_start();    //按下空格键，发送开始和暂停示波信号
        }
    }
    return QCustomPlot::event(event);
}
