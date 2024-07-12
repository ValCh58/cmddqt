#include <QDebug>
#include <memory>

#include "sport.h"
#include "config.h"
#include "ccmbb.h"

/**
 * @brief config::config
 * @param parent
 * @param f
 */
config::config(QWidget *parent, Qt::WindowFlags f):QDialog(parent, f)
{
  xmlDoc = std::make_unique<XmlDocument>("config.xml");//!!! 11-07-2024 !!!

  initDataPorts();//Чтение настроек портов из файла Config.xml//
  setWindowTitle("CMBB. Конфигурация.");
  setGeometry(492, 239, 349, 240);
  //listCom<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<"COM6"<<"COM7"<<"COM8"
  //       <<"COM9"<<"COM10"<<"COM11"<<"COM12"<<"COM13"<<"COM14"<<"COM15"<<"COM16";
  infoList = QSerialPortInfo::availablePorts();
  //-----18-06-24-------------------------------//

  //qDebug() << infoList.size();

  for(int i=0; i < infoList.size(); i++){
      //qDebug() << infoList.at(i).portName();
      listCom << infoList.at(i).portName();
  }
  //--------------------------------------------//

  helpLine1 = new QString("");
  helpLine2 = new QString("");
  //---------------------------------------------------------------//
  QLabel *lblLine1   = new QLabel("Линия 1:");
  cmbLine1= new QComboBox;
  cmbLine1->setMaxVisibleItems(16);
  initCmbBox(cmbLine1);
  cmbLine1->setCurrentIndex(XmlDocument::cfgMap.value("Line1"));
  helpInfoSerial(cmbLine1->itemText(XmlDocument::cfgMap.value("Line1")), helpLine1);
  cmbLine1->setWhatsThis(*helpLine1);
  connect(cmbLine1,SIGNAL(currentIndexChanged(int)), this, SLOT(changeLine1(int)));
  //--------------------------------------------------------------//
  QLabel *lblLine2   = new QLabel("Линия 2:");
  cmbLine2= new QComboBox;
  cmbLine2->setMaxVisibleItems(16);
  initCmbBox(cmbLine2);
  cmbLine2->setCurrentIndex(XmlDocument::cfgMap.value("Line2"));
  helpInfoSerial(cmbLine1->itemText(XmlDocument::cfgMap.value("Line2")), helpLine2);
  cmbLine2->setWhatsThis(*helpLine2);
  connect(cmbLine2,SIGNAL(currentIndexChanged(int)), this, SLOT(changeLine2(int)));
  //---------------------------------------------------------------//
  QLabel *lblBod   = new QLabel("Скорость (бит/сек):");
  cmbBod = new QComboBox;
  QStringList bodList;
  bodList<<"9600"<<"14400"<<"19200"<<"38400"<<"57600"<<"115200"<<"230400";
  cmbBod->addItems(bodList);
  int idx = XmlDocument::cfgMap.value("BaudRate");
  cmbBod->setCurrentIndex(idx);
  int bitSec = cmbBod->itemText(idx).toInt();
  XmlDocument::cfgMap.insert("BitSec",bitSec);
  connect(cmbBod,SIGNAL(currentIndexChanged(int)), this, SLOT(changeBod(int)));

  //----------------------------------------------------------------//
  QLabel *lblByteSize = new QLabel("Количество бит:");
  QSpinBox *spbByteSize = new QSpinBox();
  spbByteSize->setValue(XmlDocument::cfgMap.value("ByteSize"));
  connect(spbByteSize,SIGNAL(valueChanged(int)), this, SLOT(changeByteSize(int)));
  //----------------------------------------------------------------//
  QLabel *lblParity = new QLabel("Паритет:");
  QSpinBox *spbParity = new QSpinBox();
  spbParity->setValue(XmlDocument::cfgMap.value("Parity"));
  connect(spbParity,SIGNAL(valueChanged(int)), this, SLOT(changeParity(int)));
  //----------------------------------------------------------------//
  QLabel *lblStopBit = new QLabel("Стоп бит:");
  QSpinBox *spbStopBit = new QSpinBox();
  spbStopBit->setValue(XmlDocument::cfgMap.value("StopBit"));
  connect(spbStopBit,SIGNAL(valueChanged(int)), this, SLOT(changeStopBit(int)));
  //----------------------------------------------------------------//
  QFormLayout *layoutForm = new QFormLayout;
  //layoutForm->setVerticalSpacing(18);
  layoutForm->setRowWrapPolicy(QFormLayout::DontWrapRows);
  layoutForm->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
  layoutForm->setFormAlignment(Qt::AlignHCenter | Qt::AlignTop);
  layoutForm->setLabelAlignment(Qt::AlignRight);
  layoutForm->addRow(lblLine1, cmbLine1);
  layoutForm->addRow(lblLine2, cmbLine2);
  layoutForm->addRow(lblBod, cmbBod);
  layoutForm->addRow(lblByteSize, spbByteSize);
  layoutForm->addRow(lblParity, spbParity);
  layoutForm->addRow(lblStopBit, spbStopBit);
  QLabel *lblVer   = new QLabel("Версия ПО:");
  QLabel *lblVerMess   = new QLabel("5.0.0");
  layoutForm->addRow(lblVer, lblVerMess);
  QGroupBox *gBox = new QGroupBox("Конфигурация портов");
  QPushButton *btOk  = new QPushButton("Запустить");
  QPushButton *btCnl = new QPushButton("Отменить");
  QHBoxLayout *hBox  = new QHBoxLayout;
  hBox->addWidget(btOk);
  hBox->addWidget(btCnl);
  QVBoxLayout *mainVLayout = new QVBoxLayout;
  gBox->setLayout(layoutForm);
  mainVLayout->addWidget(gBox);
  mainVLayout->addLayout(hBox);
  setLayout(mainVLayout);
  //----------------------------------------------------------------//
  connect(btOk, SIGNAL(clicked()), this, SLOT(saveConfig()));
  connect(btCnl, SIGNAL(clicked()), this, SLOT(reject()));
}


/**
 * @brief config::~config
 */
config::~config()
{
    delete helpLine1;
    delete helpLine2;
    //delete xmlDoc; //!!! 11-07-2024 !!!
    delete cmbBod;
    delete cmbLine1;
    delete cmbLine2;
}

/**
 * @brief config::initCmbBox
 * @param cmb
 */
void config::initCmbBox(QComboBox *cmb)
{
    for(int i=0; i < listCom.size(); i++) {
        if(getInfoCom(listCom[i]))
           cmb->insertItem(i, QIcon(":/cmbbImages/network_adapter.png"), listCom[i]);
        else
           cmb->insertItem(i, QIcon(":/cmbbImages/bullet_error.png"), listCom[i]);
    }
}

/**
 * @brief config::getInfoCom
 * @param str
 * @return
 */
bool config::getInfoCom(QString str)
{
    bool ret = false;

    for(int i=0; i < infoList.size(); i++) {
        if(infoList.at(i).portName().compare(str)==0){
           ret = true;
           break;
        }
    }
    return ret;
}

/**
 * @brief config::helpInfoSerial
 * @param com
 * @param helpLine
 */
void config::helpInfoSerial(QString com, QString *helpLine)
{   helpLine->clear();
    foreach (QSerialPortInfo var, infoList) {
             if(var.portName().compare(com) == 0){
                 *helpLine += var.portName()+"\n";
                 *helpLine += var.description()+"\n";
                 *helpLine += var.manufacturer()+"\n";
                 *helpLine += var.systemLocation()+"\n";
                 *helpLine += "Занят:" + (var.isBusy() ? QObject::tr("Да") : QObject::tr("Нет"));
             }
    }
    if(helpLine->isEmpty())
        *helpLine = "Нет данных";
}

/**
 * @brief config::getNamePort
 * @param com
 * @return
 */
QString config::getNamePort(QString com)
{
    QString namePortSys = "";

    for(int i=0; i < infoList.size(); i++) {
        if(infoList.at(i).portName().compare(com)==0){
           namePortSys = infoList.at(i).systemLocation();
           break;
        }
    }
    return namePortSys;
}

/**
 * @brief config::getCmbBod
 * @return
 */
QComboBox *config::getCmbBod() const
{
    return cmbBod;
}

/**
 * @brief config::initDataPorts
 */
void config::initDataPorts()
{
    /*xmlDoc = new XmlDocument("config.xml"); //!!! 11-07-2024 !!!*/
    xmlDoc->parserCfgCom();
}

/**
 * @brief config::changeLine1
 * @param idx
 */
void config::changeLine1(int idx)
{
    XmlDocument::cfgMap["Line1"] = idx;
    helpInfoSerial(cmbLine1->itemText(XmlDocument::cfgMap.value("Line1")), helpLine1);
    cmbLine1->setWhatsThis(*helpLine1);
}

/**
 * @brief config::changeLine2
 * @param idx
 */
void config::changeLine2(int idx)
{
    XmlDocument::cfgMap["Line2"] = idx;
    helpInfoSerial(cmbLine2->itemText(XmlDocument::cfgMap.value("Line2")), helpLine2);
    cmbLine2->setWhatsThis(*helpLine2);
}

/**
 * @brief config::changeBod
 * @param idx
 */
void config::changeBod(int idx)
{
    int bitSec = cmbBod->itemText(idx).toInt();
    XmlDocument::cfgMap["BitSec"]  = bitSec;
    XmlDocument::cfgMap["BaudRate"] = idx;
}

/**
 * @brief config::changeByteSize
 * @param val
 */
void config::changeByteSize(int val)
{
    XmlDocument::cfgMap["ByteSize"] = val;
}

/**
 * @brief config::changeParity
 * @param val
 */
void config::changeParity(int val)
{
    XmlDocument::cfgMap["Parity"] = val;
}

/**
 * @brief config::changeStopBit
 * @param val
 */
void config::changeStopBit(int val)
{
    XmlDocument::cfgMap["StopBit"] = val;
}

/**
 * @brief config::saveConfig
 */
void config::saveConfig()
{
   MainWindow *mwindow = (MainWindow*)parent();
    /*xmlDoc = new XmlDocument("config.xml", this); //!!! 11-07-2024 !!!*/
    xmlDoc->saveCfgCom();

    if(!mwindow->ports.isEmpty()){
        mwindow->ports.clear();
    }
    mwindow->namePort.append(getNamePort(cmbLine1->itemText(XmlDocument::cfgMap.value("Line1"))));
    mwindow->namePort.append(getNamePort(cmbLine2->itemText(XmlDocument::cfgMap.value("Line2"))));
    mwindow->ports.append(new SPort(mwindow->namePort.at(0), this));
    mwindow->ports.append(new SPort(mwindow->namePort.at(1), this));


    connect(mwindow->ports.at(0), SIGNAL(sigError(QString)), mwindow, SLOT(printError(QString)));
    connect(mwindow->ports.at(1), SIGNAL(sigError(QString)), mwindow, SLOT(printError(QString)));
    //Вызов слотов на запись в порт////////////////////////////////////////////////////////////////////////
    connect(mwindow, SIGNAL(sigWritePort0(SPort*, int, int,int)), mwindow->ports.at(0), SLOT(writePort(SPort*, int, int, int)));
    connect(mwindow, SIGNAL(sigWritePort1(SPort*, int, int,int)), mwindow->ports.at(1), SLOT(writePort(SPort*, int, int, int)));
    //Вызов слотов на чтение из порта//////////////////////////////////////////////////////////////////////
    connect(mwindow, SIGNAL(sigReadPortAll0(SPort*,int,int,int)), mwindow->ports.at(0), SLOT(handDataSearchModules(SPort*,int,int,int)));
    connect(mwindow, SIGNAL(sigReadPortAll1(SPort*,int,int,int)), mwindow->ports.at(1), SLOT(handDataSearchModules(SPort*,int,int,int)));
    //Посылка команды ОБнаружить МОдуль//////////////////////////////////////////////////////////////////////////////////////////////////
    connect(mwindow, SIGNAL(sigMakeFindItem0(SPort*,int,int)), mwindow->ports.at(0), SLOT(detectModule(SPort*,int,int)));
    connect(mwindow, SIGNAL(sigMakeFindItem1(SPort*,int,int)), mwindow->ports.at(1), SLOT(detectModule(SPort*,int,int)));
    //Сигнал на получение ответа от ТМ и ОК//////////////////////////////////////////////////////////////////////////////////////////////
    connect(mwindow, SIGNAL(sigCheckModules0(SPort*, int, int, bool*)), mwindow->ports.at(0), SLOT(detectModuleTmOk(SPort*,int,int,bool*)));
    connect(mwindow, SIGNAL(sigCheckModules1(SPort*, int, int, bool*)), mwindow->ports.at(1), SLOT(detectModuleTmOk(SPort*,int,int,bool*)));
    //Подключение портов//////////////////
    foreach(SPort *var, mwindow->ports){
             var->connectPort();
    }

    accept();
}


