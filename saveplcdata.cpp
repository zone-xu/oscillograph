#include "saveplcdata.h"

#include <QFileDialog>
#include <QCoreApplication>
#include <QDebug>

//******************* 保存坐标数据

SavePLCData::SavePLCData()
{
    currPath = QCoreApplication::applicationDirPath() + "/oscillograph";
}

SavePLCData::~SavePLCData()
{
    if(dataFile.isOpen())
    {
        dataFile.close();
    }
}

/************************************ 线程起点执行动作 ************************************/
void SavePLCData::run()
{  
    QString fileName = "test";
    if(isModel) //如果是csv文件
    {
        qDebug() << fileName+".csv";
        dataFile.setFileName(QString(currPath + '/' + fileName+".csv"));
    }
    else {
        qDebug() << fileName+".xml";
        dataFile.setFileName(QString(currPath + '/' + fileName+".xml"));
    }

    if(!dataFile.open(QIODevice::Truncate | QIODevice::ReadWrite | QIODevice::Text))
        return;

    qDebug() << "Thread is start";
    while (isThread)
    {
        if(!taskQueue->isEmpty())   //如果坐标数据不为空
        {
            if(isStart) //如果线程处于刚创建状态
            {
                if(isModel) //csv文件保存模式
                {
                    qDebug() << "CSV is Saving InitState";
//                    dataOutPut.setDevice(&dataFile);
                    saveDataToCSVFile(dataFile);    //写入坐标数据到CSV文件
                    isStart = false;
                }
                else    //xml文件保存模式
                {
                    qDebug() << "XML is Saving InitState";
                    //创建顶层根元素
                    m_root =  saveDataToXMLFile(xmlDoc);
                    isStart = false;
                }
            }
            else
            {
                if(isModel)
                {
                    qDebug() << "CSV is Saving";
                    saveDataToCSVFile(dataFile);
                }
                else
                {
                    qDebug() << "XML is Saving";
                    appendXmlEntry(xmlDoc,m_root);    //加入xml数据（子节点）
                }
            }
        }
    }

    if(!isThread)
    {
        qDebug() << "停止线程!!!!!";
        if(!isModel)
        {
            qDebug() << "停止线程到保存XML文件";
            QTextStream out_stream(&dataFile);  //读取数据文件
            out_stream.setCodec("UTF-8");
            xmlDoc.save(out_stream,4);  //将数据保存到XML文件，缩进量4
        }
        dataFile.close();
        isStart = true;
        qDebug() << "Threading is close";
    }
    qDebug() << "end";
}

/************************************* 写入坐标数据到CSV文件 *************************************/
void SavePLCData::saveDataToCSVFile(QFile &data)
{
   plcDebug();
   QString msg = QString("%1,%2,%3,%4")
           .arg(taskQueue->first().cur_pos.time_pos)    //当前坐标的添加时刻
           .arg(taskQueue->first().name)    //变量地址
           .arg(taskQueue->first().cur_pos.x)
           .arg(taskQueue->first().cur_pos.y)+"\n";
   data.write(msg.toLocal8Bit());   //写入
   taskQueue->pop_front();  //移除刚写入的坐标数据
}

void SavePLCData::plcDebug()
{
    qDebug() <<"********************";
    qDebug() << "Y"<<taskQueue->first().cur_pos.y;
    qDebug() << "X"<<taskQueue->first().cur_pos.x;
    qDebug() << "Time" << taskQueue->first().cur_pos.time_pos;
    qDebug() << "Name" << taskQueue->first().name;
}

/************************************* 写入坐标数据到XML文件 *************************************/
QDomElement SavePLCData::saveDataToXMLFile(QDomDocument &doc)
{
    doc.clear();    //清除已有的数据
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\""); //xml文件表头
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("PLCDATA");
    doc.appendChild(root);
    return root;
}

/************************************* 加入xml数据（子节点） *************************************/
void SavePLCData::appendXmlEntry(QDomDocument &doc,QDomElement &root)
{
    QDomElement element = doc.createElement("entry");
    element.setAttribute("Y",taskQueue->first().cur_pos.y);
    element.setAttribute("X",taskQueue->first().cur_pos.x);
    element.setAttribute("Time",taskQueue->first().cur_pos.time_pos);
    element.setAttribute("Name",taskQueue->first().name);
    plcDebug();
    root.appendChild(element);
    taskQueue->pop_front(); //移除刚写入的坐标数据
}
