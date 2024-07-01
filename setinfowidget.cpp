#include "setinfowidget.h"

#define TRACKNUM 5

const QString infoWidgetStyle =
R"(QWidget
{
    color:black;
    font-family:Microsoft Yahei;
    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgb(100, 170, 200), stop:1 rgb(130, 209, 248));
}
QPushButton
{
    color:black;
    border-radius:5px;
    border:2px solid white;
    width:75px;
    height:23px;
}
QPushButton:hover {
    background-color:rgb(50, 180, 215);
}
QPushButton:pressed {
    background-color:rgb(0, 120, 215);
}
QMessageBox
{
    font: 8pt 'Microsoft Yahei';
    background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgb(255, 180, 180), stop:1 rgb(250, 150, 150));
}
QLabel {
    background:transparent;
})";

//****************************** 参数设置区域
SetInfoWidget::SetInfoWidget(QWidget *parent)
    : QFrame(parent)
{
    setStyleSheet(infoWidgetStyle);

    lLayout = new QGridLayout();  //曲线特性设置区域排布
    QLabel *typeLabel = new QLabel(QObject::tr("Variable address","typeLabel"),this);   //变量地址
    QLabel *nameLabel = new QLabel(QObject::tr("Name","nameLabel"),this);   //名称
    QLabel *colorLabel = new QLabel(QObject::tr("Color","colorLabel"),this);    //颜色
    QLabel *styleLabel = new QLabel(QObject::tr("Style Sheets","styleLabel"),this); //样式表

    //表头首行居中
    lLayout->addWidget(typeLabel,0,1,1,1,Qt::AlignCenter);
    lLayout->addWidget(nameLabel,0,2,1,1,Qt::AlignCenter);
    lLayout->addWidget(colorLabel,0,3,1,1,Qt::AlignCenter);
    lLayout->addWidget(styleLabel,0,4,1,1,Qt::AlignCenter);

    setDataNumber(TRACKNUM);    //排布曲线特性设置区域

    sureBtn = new QPushButton(QObject::tr("Sure","sureBtn"),this);  //确定按钮
    cancelBtn = new QPushButton(QObject::tr("Cancel","cancelBtn"),this);    //取消按钮

    //确定和取消按钮水平排布 在曲线特性设置区域的最后一行的最靠右位置
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(sureBtn);
    btnLayout->addWidget(cancelBtn);
    lLayout->addLayout(btnLayout,TRACKNUM+1,4);
    lLayout->setSpacing(10);
    lLayout->setContentsMargins(10,0,10,0);

    QGridLayout *rLayout = new QGridLayout();
    QFrame *line = new QFrame(this);    //分隔线1
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    line->raise();

    QFrame *line_2 = new QFrame(this);  //分隔线2
    line_2->setFrameShape(QFrame::VLine);
    line_2->setFrameShadow(QFrame::Sunken);
    line_2->raise();

    boxGroup = new QButtonGroup(rLayout);   //按钮组

    csvBox = new QCheckBox("csv",this); //csv勾选框
    xmlBox = new QCheckBox("xml",this); //xml勾选框
    xmlBox->setChecked(true);
    boxGroup->addButton(csvBox,CSV);
    boxGroup->addButton(xmlBox,XML);
    boxGroup->setExclusive(true);   //两个保存在按钮组中，只能勾选一个

    saveBtn = new QPushButton(QObject::tr("Save","saveBtn"),this);  //保存按钮
    photoBtn = new QPushButton(QObject::tr("Screenshot","photoBtn"),this);  //截图按钮
    clearBtn = new QPushButton(QObject::tr("Clear","clearBtn"),this);   //清理按钮
    importBtn = new QPushButton(QObject::tr("Import","importBtn"),this);    //导入按钮

    rLayout->addWidget(csvBox,0,0,1,1);
    rLayout->addWidget(xmlBox,0,1,1,1);
    rLayout->addWidget(saveBtn,1,0,2,2);
    rLayout->addWidget(photoBtn,3,0,2,2);
    rLayout->addWidget(clearBtn,5,0,2,2);
    rLayout->addWidget(importBtn,7,0,2,2);
    rLayout->setSpacing(5);
    rLayout->setContentsMargins(50,35,50,35);   //设置内部部件上右下左外部间距

    QVBoxLayout *rLayout_1 = new QVBoxLayout();
    timeGroup = new QButtonGroup(rLayout_1);    //文件清理时限按钮组
    notClear = new QCheckBox(QObject::tr("Not clear","notClear"),this);
    time_3Days = new QCheckBox(QObject::tr("3 days","time_3Days"),this);
    time_7Days = new QCheckBox(QObject::tr("7 days","time_7Days"),this);
    time_15Days = new QCheckBox(QObject::tr("15 days","time_15Days"),this);
    time_30Days = new QCheckBox(QObject::tr("30 days","time_30Days"),this);

    notClear->setChecked(true);

    QLabel *sampFreLab = new QLabel(this);
    sampFreLab->setText(QObject::tr("Sampling frequency", "sampFreLab"));

    m_sampFre_box = new QComboBox(this);
    QList<QString> frequencies = { "250 ms" , "500 ms", "1 s", "1.5 s", "2 s" };
    for(int i = 0; i < frequencies.size(); i++)
    {
        m_sampFre_box->addItem(frequencies[i]);
    }
    m_sampFre_box->setCurrentText("500 ms");

    timeGroup->addButton(notClear,0);
    timeGroup->addButton(time_3Days,3);
    timeGroup->addButton(time_7Days,7);
    timeGroup->addButton(time_15Days,15);
    timeGroup->addButton(time_30Days,30);
    timeGroup->setExclusive(true);

    rLayout_1->addWidget(sampFreLab);
    rLayout_1->addWidget(m_sampFre_box);
    rLayout_1->addSpacing(20);
    rLayout_1->addWidget(notClear);
    rLayout_1->addWidget(time_3Days);
    rLayout_1->addWidget(time_7Days);
    rLayout_1->addWidget(time_15Days);
    rLayout_1->addWidget(time_30Days);
    rLayout_1->setSpacing(5);
    rLayout_1->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //把所有子部件居中

    QHBoxLayout *pLayout = new QHBoxLayout();
    pLayout->addLayout(lLayout,6);
    pLayout->addWidget(line);
    pLayout->addLayout(rLayout,1);
    pLayout->addWidget(line_2);
    pLayout->addLayout(rLayout_1,1);
    setLayout(pLayout);
}

SetInfoWidget::~SetInfoWidget()
{
    delete m_sampFre_box;
    delete boxGroup; //勾选框按钮组
    delete saveBtn;   //保存按钮
    delete photoBtn;  //截图按钮
    delete clearBtn;  //清除按钮
    delete importBtn; //导入按钮
    delete timeGroup;    //文件清理时间按钮组
    delete entry_1;  //第一行曲线特性设置
    delete entry_2;  //第二行曲线特性设置
    delete entry_3;  //第三行曲线特性设置
    delete entry_4;  //第四行曲线特性设置
    delete entry_5;  //第五行曲线特性设置
}

int SetInfoWidget::getSmapFrequency()
{
    SampleFrequency frequency = static_cast<SampleFrequency>(m_sampFre_box->currentIndex());
    int sampFrequency = 500;    //默认500ms

    switch (frequency)
    {
        case MS250:
        sampFrequency = 250;
        break;

        case MS500:
        sampFrequency = 500;
        break;

        case MS1000:
        sampFrequency = 1000;
        break;

        case MS1500:
        sampFrequency = 1500;
        break;

        case MS2000:
        sampFrequency = 2000;
        break;
    }

    return sampFrequency;
}

/************************************** 排布曲线特性设置行 **************************************/
void SetInfoWidget::setDataNumber(int num)
{
    switch (num)
    {
        case 1: entry_1 = new SetEntry(this);
            lLayout->addWidget(entry_1->pcheckBox,1,0);
            lLayout->addWidget(reinterpret_cast<QLineEdit*>(entry_1->valueAddEdit),1,1);
            lLayout->addWidget(entry_1->nameEdit,1,2);
            lLayout->addWidget(entry_1->colorComboBox,1,3);
            lLayout->addWidget(entry_1->styleComboBox,1,4);
            qCheckBoxList.prepend(entry_1);
            break;
        case 2: entry_2 = new SetEntry(this);
            lLayout->addWidget(entry_2->pcheckBox,2,0);
            lLayout->addWidget(reinterpret_cast<QLineEdit*>(entry_2->valueAddEdit),2,1);
            lLayout->addWidget(entry_2->nameEdit,2,2);
            lLayout->addWidget(entry_2->colorComboBox,2,3);
            lLayout->addWidget(entry_2->styleComboBox,2,4);
            qCheckBoxList.prepend(entry_2);
            setDataNumber(1);
            break;
        case 3: entry_3 = new SetEntry(this);
            lLayout->addWidget(entry_3->pcheckBox,3,0);
            lLayout->addWidget(reinterpret_cast<QLineEdit*>(entry_3->valueAddEdit),3,1);
            lLayout->addWidget(entry_3->nameEdit,3,2);
            lLayout->addWidget(entry_3->colorComboBox,3,3);
            lLayout->addWidget(entry_3->styleComboBox,3,4);
            qCheckBoxList.prepend(entry_3);
            setDataNumber(2);
            break;
        case 4: entry_4 = new SetEntry(this);
            lLayout->addWidget(entry_4->pcheckBox,4,0);
            lLayout->addWidget(reinterpret_cast<QLineEdit*>(entry_4->valueAddEdit),4,1);
            lLayout->addWidget(entry_4->nameEdit,4,2);
            lLayout->addWidget(entry_4->colorComboBox,4,3);
            lLayout->addWidget(entry_4->styleComboBox,4,4);
            qCheckBoxList.prepend(entry_4);
            setDataNumber(3);
            break;
        case 5: entry_5 = new SetEntry(this);
            lLayout->addWidget(entry_5->pcheckBox,5,0);     //第五行，从左到右依次排布
            lLayout->addWidget(reinterpret_cast<QLineEdit*>(entry_5->valueAddEdit),5,1);
            lLayout->addWidget(entry_5->nameEdit,5,2);
            lLayout->addWidget(entry_5->colorComboBox,5,3);
            lLayout->addWidget(entry_5->styleComboBox,5,4);
            qCheckBoxList.prepend(entry_5);
            setDataNumber(4);   //转到第四行排布
            break;
    }
}
