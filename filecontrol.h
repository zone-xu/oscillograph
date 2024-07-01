#ifndef FILECONTROL_H
#define FILECONTROL_H

#include <QObject>
#include <QTime>
#include <QDebug>
#include <QFileInfoList>

//************************** 文件管理

class FileControl : public QObject
{
    Q_OBJECT
public:
    FileControl(QObject *parent = nullptr);
    ~FileControl();

    void FindFile(const QString &fileDirect,QStringList postFix, int saveDay);  //找到并删除对应路径、后缀和清理时间的文件

private:
    void DeleteFile(const QString &dir, const QString &fileName);   //删除文件

private:
    QFileInfoList *fileList;    //路径下的文件
//    QStringList filter;   //文件后缀
};

#endif // FILECONTROL_H
