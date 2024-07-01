#include "customplot.h"
#include "track.h"

const QString trackStyle =
R"(QFrame {
    background-color: rgb(44, 56, 60);
}
QLabel {
    background-color: rgb(255, 255,255);
})";

//********************** 示波区域

Track::Track(QWidget *parent)
    : QFrame(parent)
    , m_customPlot(Q_NULLPTR)
{
    QHBoxLayout *m_Layout = new QHBoxLayout(this);

    if(m_customPlot == Q_NULLPTR) {
        m_customPlot = new CustomPlot(this);
    }

    m_Layout->addWidget(m_customPlot);
    m_Layout->setContentsMargins(0,0,0,15);

    m_customPlot->setFocus();
    m_customPlot->showTracer(true); //显示示踪线

    setTitle(m_customPlot,QObject::tr("Oscillograph","m_customPlot"));  //设置标题样式
    setBackground(m_customPlot);    //设置示波区域的背景
    setLegendStyle(m_customPlot);   //设置图例（绘图小框）样式
    setAxisStyle(m_customPlot);     //设置坐标轴样式

//    setGraphStyle(m_customPlot,1);
//    setLineStyle(m_customPlot->graph(0),"PLC",1,QColor(255,255,127),SOLIDLINE);//修改线的样式
    connectInit();  //信号槽初始化
    m_customPlot->replot();

    setLayout(m_Layout);

    setStyleSheet(trackStyle);
}

Track::~Track()
{
    delete m_customPlot;   //绘画区域
}

/**************************************　勾选确定按钮：更新操作区域里的设置　**************************************/
void Track::slotUpdateSettings(QList<SetEntry*> list)
{
    if(!isRun && isClear)
    {
        isClear = false;
        m_customPlot->clearGraphs();    // 清除所有曲线
        layerNumber = 0;
        layerList.clear();
        m_customPlot->legend->setVisible(false);    //把图例设置不可见
        for(int i = 0; i < list.size(); ++i)
        {
            if(list.at(i)->dataType.isShow  && list.at(i)->getVariAddrInputState()) //如果勾选了且变量地址输入正确
            {
                layerList.append(i);    //加入勾选的行数
                m_customPlot->addGraph();   //添加数据曲线
                m_customPlot->legend->setVisible(true); //图例可见
                QString lineName = list.at(i)->dataType.valueAdd;   //添加变量地址
                QColor lineColor = list.at(i)->dataType.color;  //添加线的颜色
                LineStyle lineStyle = list.at(i)->dataType.line;    //添加线的类型
                setLineStyle(m_customPlot->graph(layerNumber++),lineName,1,lineColor,lineStyle);    //给每一个不同编号的曲线设置不同的样式
            }
        }
    }
    m_customPlot->replot(); //重绘
}

/************************************** 清除按钮:清除曲线数据　**************************************/
void Track::slotClearAll(QList<SetEntry*> list)
{
    if(isRun)
    {
        QMessageBox::warning(this,QObject::tr("Clear failed","QMessageBox"),QObject::tr("Please stop sampling first!","QMessageBox"), QMessageBox::Ok);
        return;
    }

    qDebug() << "Clear complete!";
    isClear = true;
    nCount = 0;
    layerNumber = 0;
    for(int i = 0; i < layerList.size(); ++i )
    {
        list.at(layerList.at(i))->clearData();  //清除坐标数据
        layerList.clear();
    }

//    for(int i = 0; i<m_customPlot->graphCount(); ++i)
//    {
//        m_customPlot->graph(i)->data().data()->clear();
//    }

    m_customPlot->clearGraphs();    //清除数据曲线
    m_customPlot->legend->setVisible(false);    //清除图例
    m_customPlot->replot();
}

/************************************** 截图　**************************************/
bool Track::slotPhotoBtn(QString name,int w, int h)
{
    if(m_customPlot->savePng(QCoreApplication::applicationDirPath() + "/oscillograph" + '/' + name+".png", w, h))
    {
        return true;
    }
    return false;
}

/************************************** 数据解析　**************************************/
int Track::dataParser(QString str)
{
    QString type = str.mid(0,1);    //例如x12.6,取首字母x
    QString data = str.mid(1);  //例如x12.6,取后面的数据12.6
    int num1 = 0;
    int num2 = 0;
    if(data.contains("."))
    {
        num1 = data.section(".",0,0).toInt();   //例如12.6，取首位数字12
        num2 = data.section(".",1,1).toInt();   //例如12.6，取第二位数字6
    }
    else
    {
        num1 = data.toInt();
    }

    int plc_data = 0;

    type = type.toUpper();  //把输入的首字母转大写

    if(type == "X")
    {
        plc_data = X[num1][num2];
    }
    else if(type == "Y")
    {
        plc_data = Y[num1][num2];
    }
    else if(type == "F")
    {
        plc_data = F[num1][num2];
    }
    else if(type == "G")
    {
        plc_data = G[num1][num2];
    }
    else if(type == "W")
    {
        plc_data = W[num1];
    }
    else if(type == "D")
    {
        plc_data = D[num1];
    }
    return plc_data;
}

/************************************** 添加绘图PLC数据　**************************************/
void Track::addPLCData(QList<SetEntry*> list)
{
    for(int i = 0; i < layerList.size(); ++i)
    {
        QString plcName = list.at(layerList.at(i))->dataType.valueAdd;  //获取输入的变量地址
//        int plcData = dataParser(plcName);  //获取解析的数据
        int plcData = rand()%5;//模拟测试

        POS pos;
        pos.x = nCount;     //横坐标:加入数据统计
        pos.y = plcData;    //纵坐标:PLC数据
        pos.time_pos = QTime::currentTime().toString("hh:mm:ss.zzz");

        SAVETYPE saveData;
        saveData.name = plcName;    //获取输入的变量地址
        saveData.cur_pos = pos;     //获取坐标数据
        saveQueue.push_back(saveData);

        list.at(layerList.at(i))->dataType.dataQueue.push_back(&pos);   //放入坐标数据

        m_customPlot->graph(i)->addData(pos.x,pos.y);
    }
    if(nCount > 10)
    {
        m_customPlot->xAxis->moveRange(1);  //添加10次之后，x坐标轴每次移动1
    }
    else
    {
        m_customPlot->xAxis->setRange(0,10);    //设置x坐标轴的范围
    }
    m_customPlot->graph()->rescaleValueAxis(true);  //缩放轴，只会扩大范围
    m_customPlot->replot();
    ++nCount;   //累计横坐标轴时间
}

/************************************** 读取csv文件　**************************************/
bool Track::readCSVFile(const QString &fileName)
{
    QStringList csvList;
    csvList.clear();
    QFile file(fileName);
    if(!file.open(QFile::ReadWrite))
        return false;
    QTextStream stream(&file);
    while(!stream.atEnd())  //直至读取完整
    {
        csvList.push_back(stream.readLine());   //读取文本流的一行，保存到List中
    }
    file.close();

    traversal(csvList); //加载csv不同的变量地址

    main_Vector.clear();

    for(int i = 0 ; i < namelist.size(); ++i)
    {
        LIST_VECTOR sub_vector; //定义一条数据曲线
        sub_vector.name = namelist.at(i);   //载入变量地址
        foreach(QString str,csvList.filter(namelist.at(i)))
        {
            QStringList valSplit = str.split(",");
            sub_vector.key.append(valSplit[2].toDouble());  //载入横坐标
            sub_vector.value.append(valSplit[3].toDouble());    //载入纵坐标
        }
        main_Vector.append(sub_vector); //载入数据曲线的数据
    }
    reappearLine(main_Vector);      //重现csv文件所有数据曲线

    return true;
}

/************************************** 重现csv文件所有数据曲线　**************************************/
void Track::reappearLine(QVector<LIST_VECTOR> vec)
{
    m_customPlot->clearGraphs();

    QStringList colorList;
    colorList<< "red" << "yellow" << "green" << "blue" << "white" << "cyan";

    int lineStyleType = 0;
    auto it_color = colorList.begin();

    for(int i = 0; i < vec.size(); ++i)
    {
        m_customPlot->addGraph();

        if(lineStyleType > 15) {
            lineStyleType = 0;
        }

        if(it_color == colorList.end()) {
            it_color = colorList.begin();
        }

        LineStyle lineStyle = LineStyle(lineStyleType); //创建线条形状
        QColor lineColor = QColor(*it_color);   //设置颜色

        setLineStyle(m_customPlot->graph(layerNumber++),vec.at(i).name,1,lineColor,lineStyle);  //给每一个不同编号的曲线设置不同的样式
        m_customPlot->graph(i)->addData(vec.at(i).key,vec.at(i).value); //添加横纵坐标数据
        m_customPlot->legend->setVisible(true);

        lineStyleType++;
        it_color++;
    }
    m_customPlot->replot();
}


/************************************** 信号槽连接　**************************************/
void Track::connectInit()
{
    connect(m_customPlot,&CustomPlot::signal_start,this,&Track::slotUpDate);    //开始和暂停添加数据信号切换
    connect(&timer1,&QTimer::timeout,[=]() {
    addPLCData(*qBoxList);   //进行采样
//        test = rand()%1;
//        setTrackData(m_customPlot,0,test);
    });
}

/******************************** 设置采样频率 ********************************/
void Track::slot_setFrequency(int time)
{
    timer1.setInterval(time);
}

/******************************** 设置标题样式 ********************************/
void Track::setTitle(CustomPlot *customPlot, QString str)
{
    customPlot->plotLayout()->insertRow(0);
    QCPTextElement *title = new QCPTextElement(customPlot);
    title->setText(str);
    title->setTextFlags(Qt::AlignRight);
    title->setTextColor(Qt::white);
    title->setFont(QFont("Microsoft Yahei",20,QFont::Bold));
    customPlot->plotLayout()->addElement(title);
}

/******************************** 设置示波区域的背景 ********************************/
void Track::setBackground(CustomPlot *customPlot)
{
    QLinearGradient plotGradient;   //渐变
    plotGradient.setStart(0,0);
    plotGradient.setFinalStop(0,350);
    plotGradient.setColorAt(0,QColor(98,124,133));  //设置左上角的颜色
    plotGradient.setColorAt(1,QColor(44,56,60));    //设置左下角的颜色
    customPlot->setBackground(plotGradient);
}

/******************************** 设置图例（绘图小框）样式 ********************************/
void Track::setLegendStyle(CustomPlot *customPlot)
{
    customPlot->legend->setVisible(false);  //图例（绘图小框）设置不可见
    QFont legendFont = font();
    legendFont.setPointSize(9);
    customPlot->legend->setFont(legendFont);    //设置图例的字体
    customPlot->legend->setBrush(QColor(255,255,255,30));   //设置图例背景
    customPlot->legend->setTextColor(Qt::white);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight); //设置图例的位置
}

/******************************** 设置坐标轴样式 ********************************/
void Track::setAxisStyle(CustomPlot *customPlot)
{
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));   //上下轴，左右轴范围同步
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);    //设置图元能拖拉、伸缩和可选择

//    customPlot->xAxis->setPadding(40);
    customPlot->xAxis->grid()->setPen(QPen(QColor(245,245,245,50),1,Qt::SolidLine));    //设置网格线白线
    customPlot->xAxis->setBasePen(QPen(Qt::white,1));   //基线
    customPlot->xAxis->setTickPen(QPen(Qt::gray,1));   //刻度线
    customPlot->xAxis->setSubTickPen(QPen(Qt::white,1)); //子刻度线
    customPlot->xAxis->setTickLabelColor(Qt::white);    //刻度值
    customPlot->xAxis->setLabel(QObject::tr("Time","xAxis"));    //横坐标标签
    customPlot->xAxis->setLabelColor(Qt::white);
    customPlot->xAxis->grid()->setZeroLinePen(QPen(Qt::gray,1,Qt::DashLine));   //x轴0线
    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow); //坐标轴线结尾

//    customPlot->yAxis->setPadding(40);
    customPlot->yAxis->grid()->setPen(QPen(QColor(245,245,245,50),1,Qt::SolidLine));
    customPlot->yAxis->setBasePen(QPen(Qt::white,1));
    customPlot->yAxis->setTickPen(QPen(Qt::gray,1));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white,1));
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setLabel(QObject::tr("Actual numeric value","yAxis"));
    customPlot->yAxis->setLabelColor(Qt::white);
    customPlot->yAxis->grid()->setZeroLinePen(QPen(Qt::gray,1,Qt::DashLine));
    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    customPlot->rescaleAxes(true);  //显示区域不会因为数值曲线自动调整显示
}

/******************************** 设置线条样式 ********************************/
void Track::setLineStyle(QCPGraph *graph, QString name, int fontSize, const QColor &color, LineStyle lineStyle)
{
    graph->setName(name);
    QPen pen;
    pen.setWidth(fontSize);
    pen.setColor(color);
    switch (lineStyle)
    {
        case SOLIDLINE:
            pen.setStyle(Qt::PenStyle::SolidLine);  //————
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);  //数据点用线相连接
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssNone)); //数据点无形状
            break;
        case DASHLINE:
            pen.setStyle(Qt::PenStyle::DashLine);   //----
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssNone));
            break;
        case DOTLINE:
            pen.setStyle(Qt::PenStyle::DotLine);    //·····
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssNone));
            break;
        case DASHDOTLINE:
            pen.setStyle(Qt::PenStyle::DashDotLine);    //-·-·-·
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssNone));
            break;
        case DASHDOTDOTLINE:
            pen.setStyle(Qt::PenStyle::DashDotDotLine); //-··-··
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssNone));
            break;
        case SSCROSS:
            pen.setStyle(Qt::PenStyle::SolidLine);  //————
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssCross,10)); //X
            break;
        case SSPLUS:
            pen.setStyle(Qt::PenStyle::SolidLine);  //————
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssPlus,10));  //+
            break;
        case SSCIRCLE:
            pen.setStyle(Qt::PenStyle::SolidLine);  //————
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssCircle,10));    //o
            break;
        case SSDISC:
            pen.setStyle(Qt::PenStyle::SolidLine);  //————
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssDisc,10));  //●
            break;
        case SSSQUARE:
            pen.setStyle(Qt::PenStyle::SolidLine);  //————
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssSquare,10));    //□
            break;
        case SSDIAMOND:
            pen.setStyle(Qt::PenStyle::SolidLine);  //————
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssDiamond,10));   //◇
            break;
        case SSSTAR:
            pen.setStyle(Qt::PenStyle::SolidLine);  //————
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssStar,10));  //*
            break;
        case SSTRIANGLE:
            pen.setStyle(Qt::PenStyle::SolidLine);  //————
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssTriangle,10));  //△
            break;
        case SSTRIANGLELNVERTED:
            pen.setStyle(Qt::PenStyle::SolidLine);  //————
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssTriangleInverted,10));  //▽
            break;
        case SSCROSSSQUARE:
            pen.setStyle(Qt::PenStyle::SolidLine);  //————
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssCrossSquare,10));   //☒
            break;
        case SSPLUSSQUARE:
            pen.setStyle(Qt::PenStyle::SolidLine);  //————
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssPlusSquare,10));    //+ 在 □ 里面
            break;
    }
}

bool Track::getIsClear()
{
    return isClear;
}

//void Track::setGraphStyle(CustomPlot *customPlot, int num)
//{
//    for(int i = 0 ; i < num; i++)
//    {
//        customPlot->addGraph();
//    }
//    if(customPlot->graphCount() > 0)
//    {
//        customPlot->legend->setVisible(true);
//    }
//}

/************************************** 加载csv变量地址　**************************************/
void Track::traversal(QStringList list)
{
    namelist.clear();
    foreach(QString str,list)
    {
        QStringList nameSplit = str.split(","); //分隔字符串
        QString name = nameSplit[1];    //获取变量地址
        if(!namelist.contains(name))
            namelist.append(name);  //添加不同的变量地址
    }

}

//void Track::setTrackData(CustomPlot *customPlot, int layer, double num)
//{
//    customPlot->graph(layer)->addData(nCount,num);
//    customPlot->graph()->rescaleAxes(true);
//    customPlot->scroll(1,0);
//    customPlot->replot();
//    ++nCount;
//}

/************************************** 槽函数：开始和暂停添加数据信号切换 **************************************/
void Track::slotUpDate()
{
    if(layerList.isEmpty()) { //没有加入变量地址则不进行绘制操作，弹出消息框
        QMessageBox::warning(this,QObject::tr("Data parse failed","QMessageBox"),QObject::tr("Check whether variable addresses \nare added","QMessageBox"), QMessageBox::Ok);
        return;
    }

    if(!isRun)
    {
        timer1.start(); //不在运行就开始计时
        isRun = true;
    }
    else
    {
        timer1.stop();
        isRun = false;
    }
}
