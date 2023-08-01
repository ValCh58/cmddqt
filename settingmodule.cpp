
#include "settingmodule.h"
#include "ccmbb.h"
#include "QThread"

SettingModule::SettingModule(DataModules *data, QWidget *parent, Qt::WindowFlags f):QDialog(parent, f)
{
    setWindowIcon(QIcon(":/cmbbImages/setting_tools.png"));
    MainWindow *mainWin = (MainWindow*)parent;
    sPorts = mainWin->ports;
    modul = data;
    init();
    initData();
    setWindowTitle("Настройка модуля " + modul->getTypeMod()+" "+modul->getIdMod());
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
    connect(spAdr, SIGNAL(valueChanged(int)), this, SLOT(changeAdr(int)));
    connect(btSave, SIGNAL(clicked()), this, SLOT(preParam()));
    connect(btCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(this, SIGNAL(sigSaveParam0(DataModules*,quint8,quint8,quint8)), sPorts.at(0), SLOT(saveSettingsModule(DataModules*,quint8,quint8,quint8)));
    connect(this, SIGNAL(sigSaveParam1(DataModules*,quint8,quint8,quint8)), sPorts.at(1), SLOT(saveSettingsModule(DataModules*,quint8,quint8,quint8)));
}

SettingModule::~SettingModule()
{
}


void SettingModule::init()
{
    QVBoxLayout *vBox = new QVBoxLayout();
    vBox->addWidget(hBox1("Адреса"));
    vBox->addLayout(hBox2());
    vBox->addWidget(hBox3("Линия"));
    vBox->addWidget(hBox4("Число усреднений АЦП"));
    vBox->addLayout(hBox5());
    setLayout(vBox);
}

QGroupBox *SettingModule::hBox1(QString adr)
{
   QGroupBox *gBox = new QGroupBox(adr);
   QHBoxLayout *hBox = new QHBoxLayout();
   labAdr = new QLabel("Адрес:");
   spAdr = new QSpinBox();
   spAdr->setFixedWidth(90);
   spAdr->setAlignment(Qt::AlignRight);
   labAdrHex = new QLabel("Адрес HEX:");
   edAdrHex = new QLineEdit();
   edAdrHex->setFixedWidth(90);
   edAdrHex->setAlignment(Qt::AlignRight);
   hBox->addWidget(labAdr);
   hBox->addWidget(spAdr);
   hBox->addWidget(labAdrHex);
   hBox->addWidget(edAdrHex);
   gBox->setLayout(hBox);
   return gBox;
}

QHBoxLayout *SettingModule::hBox2()
{
   QHBoxLayout *hBox = new QHBoxLayout();
   cbInv = new QCheckBox("Инверсия");
   cbRes = new QCheckBox("Резервный модуль");
   hBox->addWidget(cbInv);
   hBox->addWidget(cbRes);
   return hBox;
}

QGroupBox *SettingModule::hBox3(QString line)
{
    QGroupBox *gBox = new QGroupBox(line);
    QHBoxLayout *hBox = new QHBoxLayout();
    rbLine1 = new QRadioButton("Линия 1");
    rbLine2 = new QRadioButton("Линия 2");
    rbLine1->setChecked(true);
    hBox->addWidget(rbLine1);
    hBox->addWidget(rbLine2);
    gBox->setLayout(hBox);
    return gBox;
}

QGroupBox *SettingModule::hBox4(QString acp)
{
    QGroupBox *gBox = new QGroupBox(acp);

    QFormLayout *form1 = new QFormLayout();
    labAcp1_2 = new QLabel("АЦП 1-2:");
    spAcp1_2  = new QComboBox();
    spAcp1_2->setFixedWidth(60);
    labAcp3_4 = new QLabel("АЦП 3-4:");
    spAcp3_4  = new QComboBox();
    spAcp3_4->setFixedWidth(60);
    form1->addRow(labAcp1_2, spAcp1_2);
    form1->addRow(labAcp3_4, spAcp3_4);

    QFormLayout *form2 = new QFormLayout();
    labAcp5_6 = new QLabel("АЦП 5-6:");
    spAcp5_6  = new QComboBox();
    spAcp5_6->setFixedWidth(60);
    labAcp7_8 = new QLabel("АЦП 7-8:");
    spAcp7_8  = new QComboBox();
    spAcp7_8->setFixedWidth(60);
    form2->addRow(labAcp5_6, spAcp5_6);
    form2->addRow(labAcp7_8, spAcp7_8);

    QHBoxLayout *box = new QHBoxLayout();
    box->addLayout(form1);
    box->addLayout(form2);
    gBox->setLayout(box);
    return gBox;
}

QHBoxLayout *SettingModule::hBox5()
{
   QHBoxLayout *hbox = new QHBoxLayout();
   btSave = new QPushButton("Записать");
   btCancel = new QPushButton("Выйти");
   hbox->addWidget(btSave);
   hbox->addWidget(btCancel);
   return hbox;
}

void SettingModule::initData()
{
   spAdr->setValue(modul->getAddrDecMod().toInt());
   edAdrHex->setText(modul->getAddrHexMod());
   cbInv->setChecked(modul->getStatusInver().toInt()==0?false:true);
   cbRes->setChecked(modul->getReservMod().toInt()==0?false:true);
   if(modul->getNumInternal().toInt()==1){
      rbLine1->setChecked(true);
      rbLine2->setChecked(false);
   }else{
       rbLine2->setChecked(true);
       rbLine1->setChecked(false);
   }

   int i = modul->getSetAver().toInt();
   int j = 0;
   while(i>>=1)
         j++;

   QStringList list;
   list<<"1"<<"2"<<"4"<<"8";

   if(modul->getTypeModule()==5){
       spAcp1_2->setDisabled(true);
   }else{
       spAcp1_2->addItems(list);
       spAcp1_2->setCurrentIndex(j);
   }

   if(modul->getTypeModule()==4){
      spAcp3_4->addItems(list);
      spAcp3_4->setCurrentIndex(j);
      spAcp5_6->addItems(list);
      spAcp5_6->setCurrentIndex(j);
      spAcp7_8->addItems(list);
      spAcp7_8->setCurrentIndex(j);
   }

   if(modul->getTypeModule()!=4){
      spAcp3_4->setDisabled(true);
      spAcp5_6->setDisabled(true);
      spAcp7_8->setDisabled(true);
   }
}

void SettingModule::changeAdr(int adr)
{
   char buf[10];
   sprintf(buf, "0x%02X", adr);
   edAdrHex->setText(buf);
}

void SettingModule::preParam()
{
  quint8 adr=0, typ=0, aver=0;
  adr = (quint8)spAdr->value();
  adr |= ((rbLine2->isChecked() ? 0 : 1) << 5);
  adr |= (cbInv->isChecked() << 6);
  adr |= (cbRes->isChecked()  << 7);
  typ = modul->getTypeModule() << 4;
  aver = spAcp1_2->currentIndex();
  if(modul->getTypeModule() == 4){
     aver |= spAcp3_4->currentIndex() << 2;
     aver |= spAcp5_6->currentIndex() << 4;
     aver |= spAcp7_8->currentIndex() << 6;
  }

  int numCom = modul->getNumLine().toInt()-1;//Номер com//
  switch(numCom){
  case 0:
      sigSaveParam0(modul, adr, typ, aver);
      break;
  case 1:
      sigSaveParam1(modul, adr, typ, aver);
      break;
  }
  close();
}


