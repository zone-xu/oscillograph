#ifndef SLIDINGWINDOW_H
#define SLIDINGWINDOW_H

#include <QList>
#include <QIcon>
#include <QPushButton>
#include <QSplitter>
#include <QWidget>
#include <QtGui>
#include "track.h"
#include "setinfowidget.h"
#include "saveplcdata.h"
#include "filecontrol.h"

#include "oscillograph_global.h"

//***************** 主界面
class OSCILLOGRAPHSHARED_EXPORT SlidingWindow : public QWidget
{
    Q_OBJECT
public:
    SlidingWindow(QWidget *parent = nullptr);
    ~SlidingWindow();

protected:
    virtual void mousePressEvent(QMouseEvent* event);   //
    virtual void mouseMoveEvent(QMouseEvent* event);    //
    virtual void mouseReleaseEvent(QMouseEvent *event); //
    void resizeEvent(QResizeEvent *event);  //
    bool eventFilter(QObject *obj, QEvent *event);  //

private:
    void setBtnIcon();  //设置折叠按钮图标变化
    void setBtnPos();   //设置折叠按钮位置变化

private slots:
    void slotClickedBtn();  //点击折叠按钮导致图标变化
    void slotSplitterMoved(int pos,int index);  //分裂线变动导致折叠按钮产生相应变化
    void slotCancelBtn();   //折叠设置区域
    void slotSelectButton(int num); //选择保存数据到.csv或者是.xml文件
    void slotSelectTimer(int days); //选择清理时间

private:
    QSplitter *splitter;    //分裂线
    Track *track;   //示波区域
    SetInfoWidget *infoWidget;  //参数设置区域

    QPushButton *iconBtn;   //折叠按钮
    QIcon upIcon;
    QIcon downIcon;

    QPoint dragPosition;    //鼠标按下的位置
    bool bPressFlag;
    bool isThread;
    bool isModel;
    SavePLCData *saveThread_1;    //保存坐标数据
    FileControl *fileControl; //文件管理

};

#endif // SLIDINGWINDOW_H
