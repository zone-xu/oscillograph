#ifndef TRACK_H
#define TRACK_H

#include <QList>
#include <QTimer>
#include <QWidget>
#include "customplot.h"
#include "datatype.h"
#include "setinfowidget.h"

//********************** 电流检测区域
class Track : public QFrame
{
    Q_OBJECT

public:
    Track(QWidget *parent = nullptr);
    ~Track();

    void slotUpdateSettings(QList<SetEntry*> list); //勾选确定按钮：更新操作区域里的设置
    void slotClearAll(QList<SetEntry*> list);   //清除按钮：清除全部数据
    bool slotPhotoBtn(QString name, int w, int h);  //截图
    int dataParser(QString str);    //数据解析
    void addPLCData(QList<SetEntry*> list); //添加绘图PLC数据
    bool readCSVFile(const QString &fileName); //读取csv文件
    void reappearLine(QVector<LIST_VECTOR> vec);    //重现csv文件所有数据曲线
    bool getIsClear();    //获取正在绘制状态

public:
    CustomPlot *m_customPlot;   //绘画区域
    QList<SetEntry*> *qBoxList; //全部曲线特性设置区域
    QList<int> layerList;       //图例设置行数容器
    QQueue<SAVETYPE> saveQueue; //PLC数据存放

private:
    void connectInit(); //信号槽连接
    void setTitle(CustomPlot *customPlot, QString str); //设置标题样式
    void setBackground(CustomPlot *customPlot); //设置电流检测区域的背景
    void setLegendStyle(CustomPlot *customPlot);    //设置图例（绘图小框）样式
    void setAxisStyle(CustomPlot *customPlot);  //设置坐标轴样式
    void setLineStyle(QCPGraph *graph,QString name,int fontSize,const QColor &color,LineStyle lineStyle);   //设置线条样式
//    void setGraphStyle(CustomPlot *customPlot,int num);

    void traversal(QStringList list);   //加载csv特性名

private:
    QTimer timer1;  //加入PLC数据定时
    QList<QString> namelist;    //csv文件变量地址
    QVector<LIST_VECTOR> main_Vector;   //多条数据曲线
//    double test;

    int nCount = 0; //加入数据统计
    int layerNumber = 0;    //设置图例的数目
//    int switchIndex = 0;

    bool isRun = false;     //是否正在绘制
    bool isClear = true;    //是否清理完毕？

    //plc参数
    bool X[128][8];
    bool Y[128][8];
    bool F[128][8];
    bool G[128][8];
    short W[200];
    int D[100];

public slots:
    void slot_setFrequency(int time);    //设置采样频率

private slots:
//    void setTrackData(CustomPlot *customPlot, int layer, double num);
    void slotUpDate();  //槽函数：开始和暂停添加数据信号切换

};

#endif // TRACK_H
