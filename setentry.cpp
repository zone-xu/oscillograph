#include "setentry.h"

#include "segmentcheckedit/addrsegmentcheckedit.h"

#include <QRegExpValidator>
#include <QMetaEnum>
#include <typeinfo>
#include <QThread>
#include <QTime>
#include <QApplication>

SetEntry::SetEntry(QWidget *parent)
    : QWidget(parent)
    ,  pcheckBox(Q_NULLPTR)
    ,  valueAddEdit(Q_NULLPTR)
//    ,  typeComboBox(Q_NULLPTR)
    ,  colorComboBox(Q_NULLPTR)
    , styleComboBox(Q_NULLPTR)
{
//    this->setStyleSheet("background-color:#2c383c");

    typeMapToNum =
    {
        { "X", 1 },
        { "Y", 1 },
        { "F", 1 },
        { "G", 1 },
        { "W", 2 },
        { "D", 3 }
    };   //地址类型对应的数字

    QStringList colorList;
    colorList << QObject::tr("Red") << QObject::tr("Yellow") << QObject::tr("Green") << QObject::tr("Blue") << QObject::tr("White") << QObject::tr("Cyan");
    QStringList colorList_1;
    colorList_1 << "Red" << "Yellow" << "Green" << "Blue" << "White" << "Cyan";

    QStringList styleList;
    styleList << "SOLIDLINE" << "DASHLINE" << "DOTLINE" << "DASHDOTLINE"
              << "DASHDOTDOTLINE" << "SSCROSS" << "SSPLUS" << "SSCIRCLE"
              << "SSDISC" << "SSSQUARE" << "SSDIAMOND" << "SSSTAR" << "SSTRIANGLE"
              << "SSTRIANGLELNVERTED" << "SSCROSSSQUARE" << "SSPLUSSQUARE";

    pcheckBox = new QCheckBox(this);
    dataType.isShow = false;

    valueAddEdit = new AddrSegmentCheckEdit(false,this);    //非模态，不阻塞程序

    /******** bool X[128][8]、bool Y[128][8]、bool F[128][8]、bool G[128][8] ********/

    /******** short W[200]、int D[100] ********/

    /************************* 变量地址输入格式 *************************/
    valueAddEdit->setPattern("^((([XYFG])([^\\.]*|([1-9]?\\d|1[0-1]\\d|12[0-7]))((\\.)([^\\.]*|([0-7]))?)?)|(([W])(.*|([1-9]?\\d|1\\d\\d)))|(([D])(.*|([1-9]?\\d))))$");

    nameEdit = new QLineEdit(this);
    nameEdit->setEnabled(false);

    colorComboBox = new QComboBox(this);
    for(int j = 0; j < colorList.size(); ++j)
    {
        QColor color(colorList_1.at(j));
        colorComboBox->addItem(colorSwitch(color),colorList.at(j),color);
    }
    dataType.color = QColor(colorComboBox->currentData().toString());

    styleComboBox = new QComboBox(this);
    styleComboBox->addItems(styleList);
    dataType.line = LineStyle(styleComboBox->currentIndex());

    connect(pcheckBox,&QCheckBox::clicked,this,&SetEntry::updateShow);
    connect(valueAddEdit,&QLineEdit::textChanged,this,&SetEntry::errorTextFind);
    connect(valueAddEdit,&QLineEdit::editingFinished,this,&SetEntry::updateValue);
    connect(nameEdit,SIGNAL(editingFinished()),this,SLOT(updateName()));
    connect(colorComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateColor(QString)));
    connect(styleComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateStyle(int)));
}

SetEntry::~SetEntry()
{
    delete pcheckBox;
    delete valueAddEdit;
    delete nameEdit;
    delete colorComboBox;
    delete styleComboBox;
}

/************************************** 清除坐标数据　**************************************/
void SetEntry::clearData()
{
    dataType.dataQueue.clear();
}

bool SetEntry::getVariAddrInputState()
{
    return valueAddEdit->getInputState();
}

QPixmap SetEntry::colorSwitch(const QColor &color)
{
    QPixmap pixmap(15,15);
    pixmap.fill(color);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.drawRect(0,0,5,5);
    painter.end();
    return pixmap;
}

void SetEntry::updateShow(bool vis)
{
    dataType.isShow = vis;
    dataType.valueAdd = valueAddEdit->text();
    dataType.name = nameEdit->text();
//    dataType.tpye = TypeStyle(typeComboBox->currentIndex());
    dataType.color = QColor(colorComboBox->currentData().toString());
    dataType.line = LineStyle(styleComboBox->currentIndex());
}

void SetEntry::updateValue()
{
    dataType.valueAdd = valueAddEdit->text();
    valueAddEdit->completeTip();
}

//void SetEntry::updataType(int num)
//{
//    dataType.tpye = TypeStyle(num);

//}

void SetEntry::updateName()
{
    dataType.name = nameEdit->text();
}

void SetEntry::updateColor(QString str)
{
    Q_UNUSED(str);
    dataType.color = QColor(colorComboBox->currentData().toString());
}

void SetEntry::updateStyle(int num)
{
    dataType.line = LineStyle(num);
}

/***************************** 查找错误文本 ******************************/
void SetEntry::errorTextFind(const QString &text)
{
    //空字符隐藏错误提示框
    if(text == "")
    {
        valueAddEdit->hideErrorTipBox();
        return;
    }

    int type = typeMapToNum[text.at(0).toUpper()];  //捕获输入的首字母

    switch (type)
    {
        case 1: //已捕获X|Y|F|G
            valueAddEdit->addSegmentPositons(3,5,7,9);
            valueAddEdit->addValueRange("X|Y|F|G","0-127","0-7");
            valueAddEdit->errorTextFind(text);
        break;
        case 2: //已捕获 W
            valueAddEdit->addSegmentPositons(11,13);
            valueAddEdit->addValueRange("W","0-199");
            if(text.indexOf('.') != -1 ) {
                valueAddEdit->appendErrorTip(QObject::tr("\nIf you want to input the address type is 'W',\nplease delete the point->'.' ","valueAddEdit"));
            }
            else {
                valueAddEdit->appendErrorTip(QObject::tr(""));
            }
            valueAddEdit->errorTextFind(text);
        break;
        case 3: //已捕获 D
            valueAddEdit->addSegmentPositons(15,17);
            valueAddEdit->addValueRange("D","0-99");
            if(text.indexOf('.') != -1) {
                valueAddEdit->appendErrorTip(QObject::tr("\nIf you want to input the address type is 'D',\nplease delete the point->'.'", "valueAddEdit"));
            }
            else {
                valueAddEdit->appendErrorTip(QObject::tr(""));
            }
            valueAddEdit->errorTextFind(text);
        break;
        default: //其他字母
            valueAddEdit->showErrorTipText(QObject::tr("address type","valueAddEdit"), "X|Y|F|G|W|D",5000);
        break;
    }

}
