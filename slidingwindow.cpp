#include "slidingwindow.h"
#include <QApplication>
#include <QDesktopWidget>

//***************** 主界面
SlidingWindow::SlidingWindow(QWidget *parent)
    : QWidget(parent)
    , bPressFlag(false)
    , isThread(false)
    , isModel(false)
    , saveThread_1(nullptr)
    , fileControl(nullptr)
{
    QString currPath = QCoreApplication::applicationDirPath() + "/oscillograph";
    QDir dir;
    if(!dir.exists(currPath))
    {
        dir.mkdir(currPath);
    }

    resize(931,631);
//    this->setMaximumSize(931,631);
    splitter = new QSplitter(Qt::Vertical,this);
    splitter->setHandleWidth(1);
    splitter->setStyleSheet("QSplitter::handle{background-color:#ffffff}");

    track = new Track(splitter);    //示波显示在分割器上方
    track->resize(width(),400);
    track->setMinimumHeight(360);

    infoWidget = new SetInfoWidget(splitter);   //参数设置区域在分割器下方
    infoWidget->resize(width(),631-400);
    infoWidget->setMaximumHeight(631-360);

    track->qBoxList = &infoWidget->qCheckBoxList;   //把勾选的曲线数据加入到示波区域

    connect(infoWidget->sureBtn,&QPushButton::clicked,[=](){
       track->slotUpdateSettings(infoWidget->qCheckBoxList);    //勾选确定按钮:更新操作区域里的设置
       track->slot_setFrequency(infoWidget->getSmapFrequency());//更新采样频率
    });
    connect(infoWidget->clearBtn,&QPushButton::clicked,[=](){
       track->slotClearAll(infoWidget->qCheckBoxList);  //清除按钮:清除曲线数据

    });
    connect(infoWidget->photoBtn,&QPushButton::clicked,[=](){
        if(track->slotPhotoBtn("customPlot",1280,960))
            qDebug() << "Screenshot succeed!";  //截图 ->示波区域
    });

    QString applicationDirPath;
    applicationDirPath = QCoreApplication::applicationDirPath();    //获取应用程序路径

    connect(infoWidget->importBtn,&QPushButton::clicked,[=]()
    {
        if(!track->getIsClear())
        {
            QMessageBox::warning(this,QObject::tr("Import failed","QMessageBox"),QObject::tr("Please clear first!","QMessageBox"), QMessageBox::Ok);
            return;
        }
        else
        {
            QString fileName;
            fileName = QFileDialog::getOpenFileName(this,tr("File import"),applicationDirPath,"CSV files (*.csv)");  //返回一个用户选中的csv文件
            if(track->readCSVFile(fileName)) //读取csv文件，显示文件中的数据曲线
                qDebug() << "Import succeed!";
        }
    });


    connect(infoWidget->saveBtn,&QPushButton::clicked,[=](){
        isThread = !isThread;   //切换开始保存和结束保存坐标数据两种状态
        if(isThread)
        {
            qDebug() << "start";
            if(saveThread_1 == nullptr)
            {
                saveThread_1 = new SavePLCData();
                saveThread_1->taskQueue = &track->saveQueue;    //存放坐标数据地址
            }
            saveThread_1->isThread = isThread;
            saveThread_1->isModel = isModel;
            saveThread_1->start();  //开启线程
        }
        else
        {
            qDebug() << "stop";
            saveThread_1->isThread = isThread;
            qDebug() << "thread" << saveThread_1->isThread;
            qDebug() << "isThread is quit";

            if (saveThread_1->isStart)
            {
                saveThread_1->quit();   //停止线程
                saveThread_1->wait();   //等待线程处理完手头工作
            }
        }
    });

    fileControl = new FileControl(this);
    connect(infoWidget->boxGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotSelectButton(int)));  //选择保存数据到.csv或者是.xml文件
    connect(infoWidget->timeGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotSelectTimer(int)));  //选择清理时间

    connect(infoWidget->cancelBtn,&QPushButton::clicked,this,&SlidingWindow::slotCancelBtn);    //折叠设置区域
    connect(splitter,SIGNAL(splitterMoved(int,int)),this,SLOT(slotSplitterMoved(int,int))); //分裂线变动导致折叠按钮产生相应变化

    upIcon = QIcon(":/images/Up.png");
    downIcon = QIcon(":/images/Down.png");

     //该按钮显示位置出错，进行隐藏了
    iconBtn = new QPushButton(this);
    iconBtn->setFocusPolicy(Qt::NoFocus);
    iconBtn->hide();
    iconBtn->setFixedSize(42,20);
    iconBtn->setIconSize(iconBtn->size());
    iconBtn->setStyleSheet("border:none");
    iconBtn->setIcon(downIcon);
//    iconBtn->move((track->width() - iconBtn->width())/2,track->height() - iconBtn->height());
    connect(iconBtn,SIGNAL(clicked()),this,SLOT(slotClickedBtn())); //点击折叠按钮导致图标变化

    track->setMouseTracking(true);
    infoWidget->setMouseTracking(true);
    installEventFilter(this);
}

SlidingWindow::~SlidingWindow()
{
    delete iconBtn;   //折叠按钮

    if(saveThread_1 != nullptr)
    {
        saveThread_1->quit();
        saveThread_1->wait();   //等待线程处理完手头工作
        delete saveThread_1;    //保存坐标数据
    }

    delete splitter;    //分裂线
    delete fileControl; //文件管理
}

/*************************************** 设置折叠按钮图标变化 ***************************************/
void SlidingWindow::setBtnIcon()
{
    if(infoWidget->height() != 0) {
        iconBtn->setIcon(downIcon);
    }
    else {
        iconBtn->setIcon(upIcon);
    }
}

/*************************************** 设置折叠按钮位置变化 ***************************************/
void SlidingWindow::setBtnPos()
{
    iconBtn->move(track->width()/2 - iconBtn->width()/2 , track->height() - iconBtn->height()); //移动到示波区域中间
}

/*************************************** 点击折叠按钮导致图标变化 ***************************************/
void SlidingWindow::slotClickedBtn()
{
    QList <int> sizeList;
    sizeList.clear();
    if(infoWidget->height() != 0)
    {
        sizeList.append(631);
        sizeList.append(0);
    }
    else
    {
        sizeList.append(400);
        sizeList.append(631-400);
    }
    splitter->setSizes(sizeList);

    setBtnIcon();
    setBtnPos();
    iconBtn->hide();

}

/*************************************** 分裂线变动导致折叠按钮产生相应变化 ***************************************/
void SlidingWindow::slotSplitterMoved(int pos, int index)
{
    Q_UNUSED(pos)
    Q_UNUSED(index)
    setBtnIcon();
    setBtnPos();
}

/*************************************** 折叠设置区域 ***************************************/
void SlidingWindow::slotCancelBtn()
{
    QList <int> sizeList;
    sizeList.clear();
    if(infoWidget->height() != 0)
    {
        sizeList.append(631);
        sizeList.append(0);
    }
    splitter->setSizes(sizeList);
    setBtnIcon();
    setBtnPos();
    iconBtn->hide();
}

/*************************************** 选择保存数据到.csv或者是.xml文件 ***************************************/
void SlidingWindow::slotSelectButton(int num)
{  
    if(num == CSV)
    {      
        if(!isThread)   //如果不是在记录坐标数据的过程中选择保存文件为CSV文件类型
        {
            qDebug()<<"csv";
            isModel = true; //更改保存文件类型为CSV
        }
        else    //如果在记录坐标数据的过程中选择保存文件为CSV文件类型
        {
            qDebug() << "Invalid change operation!";    //无法更改文件类型
            infoWidget->xmlBox->setChecked(true);
        }
    }
    else
    {
        if(!isThread)   //如果不是在记录坐标数据的过程中选择保存文件为xml文件类型
        {
            qDebug() <<"xml";
            isModel = false;    //更改保存文件类型为xml
        }
        else    //如果在记录坐标数据的过程中选择保存文件为xml文件类型
        {
            qDebug() << "Invalid change operation!";    //无法更改文件类型
            infoWidget->csvBox->setChecked(true);
        }
    }
}

/*************************************** 选择清理时间 ***************************************/
void SlidingWindow::slotSelectTimer(int days)
{
    QStringList filter;
    filter<<"*.csv"<<"*.xml";
    QString path = QCoreApplication::applicationDirPath() + "/oscillograph";
    switch (days)
    {
        case 3: fileControl->FindFile(path,filter,3);
            break;
        case 7: fileControl->FindFile(path,filter,7);
            break;
        case 15:fileControl->FindFile(path,filter,15);
            break;
        case 30:fileControl->FindFile(path,filter,30);
            break;
        default:
            break;
    }
}

/*************************************** 鼠标拖拽事件:按下 ***************************************/
void SlidingWindow::mousePressEvent(QMouseEvent * event)
{
    bPressFlag = true;
    dragPosition = event->pos();    //记下鼠标按下的位置
    QWidget::mousePressEvent(event);
}

/*************************************** 鼠标拖拽事件:移动 ***************************************/
void SlidingWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(bPressFlag)
    {
        QPoint relaPos(QCursor::pos() - dragPosition);  //记录鼠标按下移动时，拽拉窗体的距离
        move(relaPos);  //移动窗体
    }
    QWidget::mouseMoveEvent(event);
}

/*************************************** 鼠标拖拽事件:释放 ***************************************/
void SlidingWindow::mouseReleaseEvent(QMouseEvent *event)
{
    bPressFlag = false;
    QWidget::mouseReleaseEvent(event);
}

/*************************************** 窗体放大事件 ***************************************/
void SlidingWindow::resizeEvent(QResizeEvent *event)
{
    splitter->setGeometry(0, 0, width(), height());
    setBtnPos();    //每次放大都改变下拉、上拉箭头按钮的位置
    QWidget::resizeEvent(event);
}

/*************************************** 事件过滤器 ***************************************/
bool SlidingWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseMove = static_cast<QMouseEvent*>(event);
        QRect rect = iconBtn->frameGeometry();  //获取按键的几何位置
        if (rect.contains(mouseMove->pos()))    //若移到按键位置范围
        {
            iconBtn->show();    //按键显示
        }
        else
        {
            iconBtn->hide();   //按键隐藏
        }
    }
    return QWidget::eventFilter(obj, event);
}
