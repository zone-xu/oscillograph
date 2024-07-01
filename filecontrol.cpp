#include "filecontrol.h"

#include <QDir>
#include <QFileInfo>

//************************** 文件管理
FileControl::FileControl(QObject *parent)
    : QObject(parent)
{
//    filter << "*.csv" << "*.xml";
//    QString path = "E:/build-PLC_Track-Desktop_Qt_5_9_8_MinGW_32bit-Debug";
    //    FindFile(path,filter,1);
}

FileControl::~FileControl()
{

}

/************************************** 找到并删除对应路径、后缀和清理时间的文件 **************************************/
void FileControl::FindFile(const QString &fileDirect, QStringList postFix, int saveDay)
{
    QDateTime current_date_time = QDateTime::currentDateTime(); //获取当前的日期
//    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");

    QFileInfo dirInfo(fileDirect);  //获取文件路径
    if(!dirInfo.isReadable())   //如果文件路径不存在
    {
        qDebug() << "File or directory is not readable or doesn't exist.";
        return;
    }

    fileList = new QFileInfoList;
    fileList->clear();
    *fileList = QDir(fileDirect).entryInfoList(postFix);    //找寻fileDirect文件路径下postfix后缀的文件

    for(int i = 0;i < fileList->size();++i)
    {
       QFileInfo *fileInfo = new QFileInfo(fileList->at(i));
       qDebug() << fileInfo->fileName();
       qDebug() << fileInfo->lastModified();
       QDateTime startDate(fileInfo->lastModified());
       QDateTime endDate(current_date_time);
       int sDays = static_cast<int>(startDate.daysTo(endDate));   //得到是第几天之前的文件
       qDebug() << "SaveToDays:" << startDate.daysTo(endDate);
       if(sDays >= saveDay)
       {
           DeleteFile(fileDirect,fileInfo->fileName()); //大于清理时间就删除文件
           qDebug()<<"文件已删除";
       }
    }
}


/************************************** 删除文件 **************************************/
void FileControl::DeleteFile(const QString &dir,const QString &fileName)
{
    qDebug() << "************************";
    qDebug() << "被删除文件地址："<< dir;
    qDebug() << "被删除文件名字" << fileName;
    if(QDir(dir).remove(fileName))
    {
        qDebug()<<"Deleting file is success.";
    }
    else
    {
        qDebug()<<"Deleting file is failure.";
    }
}
