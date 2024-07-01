#ifndef SETENTRY_H
#define SETENTRY_H

#include "datatype.h"

#include <QDebug>
#include <QPainter>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QWidget>
#include <QStandardItemModel>
#include <QCompleter>
#include <QMessageBox>

class AddrSegmentCheckEdit;

class SetEntry : public QWidget
{
    Q_OBJECT
public:
    SetEntry(QWidget *parent = nullptr);
    ~SetEntry();

public:
    void clearData();   //清除
    bool getVariAddrInputState();   //获取变量地址输入状态。true:输入正确

public:
    QCheckBox *pcheckBox;
    AddrSegmentCheckEdit *valueAddEdit;
    QLineEdit *nameEdit;
    QComboBox *colorComboBox;
    QComboBox *styleComboBox;
    DATATYPE dataType;  //曲线特性

private:
    QMap<QString,int> typeMapToNum; //地址类型对应的数字

private:
    QPixmap colorSwitch(const QColor &color);

private slots:
    void updateShow(bool vis);
//    void updataType(int num);
    void updateValue();
    void updateName();
    void updateColor(QString str);
    void updateStyle(int num);
    void errorTextFind(const QString &text);    //查找错误文本
};

#endif // SETENTRY_H
