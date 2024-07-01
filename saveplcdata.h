#ifndef SAVEPLCDATA_H
#define SAVEPLCDATA_H

#include <QDomDocument>
#include <QTextStream>
#include <QTime>
#include <QDebug>
#include <QFile>
#include <QThread>
#include <QQueue>
#include "datatype.h"


//******************* 保存坐标数据
class SavePLCData : public QThread
{
    Q_OBJECT
public:
    SavePLCData();
    ~SavePLCData();

public:
    bool isStart = true;    //线程刚创建状态
    bool isModel = false;   //文件类型：csv和xml。true:csv文件
    bool isThread = false;  //线程状态
    QQueue<SAVETYPE> *taskQueue = nullptr;  //存放坐标数据

private:
    void saveDataToCSVFile(QFile &data);    //写入坐标数据到CSV文件
    void plcDebug();
    QDomElement saveDataToXMLFile(QDomDocument &doc);   //将数据保存到XML文件
    void appendXmlEntry(QDomDocument &doc, QDomElement &root);  //加入xml数据（子节点）

private:
    QFile dataFile;
    QString currPath;   //文件存放路径

    QDomDocument xmlDoc;    //xml文件
    QDomElement m_root;       //xml根元素

protected:
    virtual void run(); //线程起点执行动作
};

#endif // SAVEPLCDATA_H
