#ifndef DATATYPE_H
#define DATATYPE_H

#include <QVector>
#include <QTimer>
#include <QColor>
#include <QQueue>

typedef enum{
    SOLIDLINE,
    DASHLINE,
    DOTLINE,
    DASHDOTLINE,
    DASHDOTDOTLINE,
    SSCROSS,
    SSPLUS,
    SSCIRCLE,
    SSDISC,
    SSSQUARE,
    SSDIAMOND,
    SSSTAR,
    SSTRIANGLE,
    SSTRIANGLELNVERTED,
    SSCROSSSQUARE,
    SSPLUSSQUARE
}LineStyle;

typedef enum{
    X,
    Y,
    F,
    G,
    W,
    D
}TypeStyle;

typedef enum{
    CSV = 1,
    XML = 2,
}FileType;

//当前坐标位置和时刻
struct POS {
    int x = 0;
    int y = 0;
    QString time_pos;   //获取当前坐标的添加时刻
};

//曲线特性
typedef struct dataType {
    bool isShow;
    QString valueAdd;
    QString name;
    QColor color;
    LineStyle line;
    QQueue<POS*> dataQueue; //存放坐标数据
}DATATYPE;

//当前坐标数据
typedef struct saveType {
    QString name;   //变量地址
    POS cur_pos;    //当前坐标位置和时刻
}SAVETYPE;

//一条数据曲线
typedef struct list_vector{
    QString name;
    QVector<double> key;    //横坐标
    QVector<double> value;  //纵坐标
}LIST_VECTOR;


#endif // DATATPYE_H
