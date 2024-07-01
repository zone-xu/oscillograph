#ifndef SETINFOWIDGET_H
#define SETINFOWIDGET_H

#include <QButtonGroup>
#include <QLabel>
#include <QGridLayout>
#include <QWidget>
#include <QPushButton>
#include "setentry.h"
#include "datatype.h"

//采样频率
enum SampleFrequency
{
    MS250 = 0,  //250ms
    MS500 = 1,
    MS1000 = 2,
    MS1500 = 3,
    MS2000 = 4,
};

//****************************** 参数设置区域
class SetInfoWidget : public QFrame
{
    Q_OBJECT
public:
    SetInfoWidget(QWidget *parent = nullptr);
    ~SetInfoWidget();

public:
    int getSmapFrequency(); //获取采样频率

private:
    void setDataNumber(int num);    //排布曲线特性设置行

public:
    QList<SetEntry *> qCheckBoxList;    //全部曲线特性设置区域
    QPushButton *sureBtn;   //确定按钮
    QPushButton *cancelBtn; //取消按钮

    QButtonGroup *boxGroup; //勾选框按钮组
    QCheckBox *csvBox;  //csv勾选框
    QCheckBox *xmlBox;  //xml勾选框

    QPushButton *saveBtn;   //保存按钮
    QPushButton *photoBtn;  //截图按钮
    QPushButton *clearBtn;  //清除按钮
    QPushButton *importBtn; //导入按钮

    QComboBox *m_sampFre_box;   //采样频率下拉选择框

    QButtonGroup *timeGroup;    //文件清理时间按钮组
    QCheckBox *notClear;    //不清理
    QCheckBox *time_3Days;  //3天以上则清理
    QCheckBox *time_7Days;  //7天以上则清理
    QCheckBox *time_15Days; //15天以上则清理
    QCheckBox *time_30Days; //30天以上则清理

private:
    QGridLayout *lLayout;  //曲线特性设置区域排布

    SetEntry *entry_1;  //第一行曲线特性设置
    SetEntry *entry_2;  //第二行曲线特性设置
    SetEntry *entry_3;  //第三行曲线特性设置
    SetEntry *entry_4;  //第四行曲线特性设置
    SetEntry *entry_5;  //第五行曲线特性设置
};

#endif // SETINFOWIDGET_H
