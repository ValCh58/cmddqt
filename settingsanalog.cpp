#include <QIcon>
#include <QTextCodec>
#include <QThread>

#include "datamodules.h"
#include "settingsanalog.h"
#include "ccmbb.h"



SettingsAnalog::SettingsAnalog(DataModules *data, QWidget *parent, Qt::WindowFlags f):QDialog(parent,f)
{
    setWindowIcon(QIcon(":/cmbbImages/ustav.png"));
    MainWindow *mainWin = (MainWindow*)parent;
    sPorts = mainWin->ports;
    modul = data;
    setModal(true);
    sbar = new QStatusBar(this);
    sbar->setFixedHeight(10);
    init();
    mds = mainWin->getMapAdcPloAddr();
    adcs = mainWin->getMapAdcs();
    setWindowTitle("Задать уставки модуля " + modul->getTypeMod()+" "+modul->getIdMod());
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());

    connect(this, SIGNAL(sigReadSettings0(DataModules*,uint*,uint*)), sPorts.at(0), SLOT(readSettingsAcp(DataModules*,uint*,uint*)));
    connect(this, SIGNAL(sigReadSettings1(DataModules*,uint*,uint*)), sPorts.at(1), SLOT(readSettingsAcp(DataModules*,uint*,uint*)));
    readSettings();

    connect(btClose, SIGNAL(pressed()), this, SLOT(close()));
    connect(btWrite, SIGNAL(pressed()), this, SLOT(writeSettings()));
    connect(this, SIGNAL(sigWriteSettings0(DataModules*,uint*,uint*)), sPorts.at(0), SLOT(writeSettingsAcp(DataModules*,uint*,uint*)));
    connect(this, SIGNAL(sigWriteSettings1(DataModules*,uint*,uint*)), sPorts.at(1), SLOT(writeSettingsAcp(DataModules*,uint*,uint*)));
}


SettingsAnalog::~SettingsAnalog()
{
    if(mds)
       delete mds;
    if(adcs)
       delete adcs;
}

//Запись уставок////////////////////////////////////////////
void SettingsAnalog::writeSettings()
{
    setDisBtn(true);
    repaint();
    readFromArrSet();
    vProgrBar->progress->setValue(50);
    repaint();
    int numCom = modul->getNumLine().toInt()-1;//Номер линии//

    switch(numCom){
    case 0:
        emit sigWriteSettings0(modul, lowVar, hiVar);
        break;
    case 1:
        emit sigWriteSettings1(modul, lowVar, hiVar);
        break;
    }
    vProgrBar->progress->setValue(75);
    repaint();
    QThread::usleep(4000*1000);
    //vProgrBar->progress->setValue(80);
    readSettings();
    vProgrBar->progress->setValue(100);
    setDisBtn(false);
}



void SettingsAnalog::readFromArrSet()
{
    lowVar[0] = edAcp1Low->text().toUInt();
    hiVar[0] = edAcp1Hi->text().toUInt();
    lowVar[1] = edAcp2Low->text().toUInt();
    hiVar[1] = edAcp2Hi->text().toUInt();
    if(modul->getTypeModule() == 4){
        lowVar[2] = edAcp3Low->text().toUInt();
        hiVar[2] = edAcp3Hi->text().toUInt();
        lowVar[3] = edAcp4Low->text().toUInt();
        hiVar[3] = edAcp4Hi->text().toUInt();
        lowVar[4] = edAcp5Low->text().toUInt();
        hiVar[4] = edAcp5Hi->text().toUInt();
        lowVar[5] = edAcp6Low->text().toUInt();
        hiVar[5] = edAcp6Hi->text().toUInt();
        lowVar[6] = edAcp7Low->text().toUInt();
        hiVar[6] = edAcp7Hi->text().toUInt();
        lowVar[7] = edAcp8Low->text().toUInt();
        hiVar[7] = edAcp8Hi->text().toUInt();
    }
}

void SettingsAnalog::setDisBtn(bool flag)
{
    btWrite->setDisabled(flag);
    btClose->setDisabled(flag);
}

//Чтение уставок////////////////////////////////////////////
void SettingsAnalog::readSettings()
{
  int numCom = modul->getNumLine().toInt()-1;//Номер линии//

  switch(numCom){
  case 0:
      emit sigReadSettings0(modul, lowVar, hiVar);
      break;
  case 1:
      emit sigReadSettings1(modul, lowVar, hiVar);
      break;
  }
  fillForm_1_8();
  fillLabel();
}

void SettingsAnalog::fillForm_1_8()
{
    edAcp1Low->setText(QString::number(lowVar[0]));
    edAcp1Hi->setText(QString::number(hiVar[0]));
    edAcp2Low->setText(QString::number(lowVar[1]));
    edAcp2Hi->setText(QString::number(hiVar[1]));
    if(modul->getTypeModule() == 4){
       edAcp3Low->setText(QString::number(lowVar[2]));
       edAcp3Hi->setText(QString::number(hiVar[2]));
       edAcp4Low->setText(QString::number(lowVar[3]));
       edAcp4Hi->setText(QString::number(hiVar[3]));
       edAcp5Low->setText(QString::number(lowVar[4]));
       edAcp5Hi->setText(QString::number(hiVar[4]));
       edAcp6Low->setText(QString::number(lowVar[5]));
       edAcp6Hi->setText(QString::number(hiVar[5]));
       edAcp7Low->setText(QString::number(lowVar[6]));
       edAcp7Hi->setText(QString::number(hiVar[6]));
       edAcp8Low->setText(QString::number(lowVar[7]));
       edAcp8Hi->setText(QString::number(hiVar[7]));
    }
    if(modul->getTypeModule() != 4){
       edAcp3Low->setDisabled(true);
       edAcp4Low->setDisabled(true);
       edAcp5Low->setDisabled(true);
       edAcp6Low->setDisabled(true);
       edAcp7Low->setDisabled(true);
       edAcp8Low->setDisabled(true);
       edAcp3Hi->setDisabled(true);
       edAcp4Hi->setDisabled(true);
       edAcp5Hi->setDisabled(true);
       edAcp6Hi->setDisabled(true);
       edAcp7Hi->setDisabled(true);
       edAcp8Hi->setDisabled(true);
    }

}

void SettingsAnalog::fillLabel()
{
   QString s1 = "&lsaquo; &nbsp;";
   QString s2 = "&nbsp; &lsaquo;";
   QString st1="<span style=\" font-size:12pt; font-weight:600; color:blue;\" > ";
   QString st2="<span style=\" font-size:12pt; font-weight:600; color:red;\" > ";
   st1.append(s1);
   st1.append("</span>");
   st2.append(s2);
   st2.append("</span>");


   Adcs ad;
   QString key = modul->getAddrHexMod().right(modul->getAddrHexMod().length()-2)+modul->getNumLine();
   AdcPloAddr apa = mds->value(key);
   ad = adcs->value(QString::number(apa.getAdcAddr()));
   shortName1->setText(st1+ad.getShortName()+st2);

   ad = adcs->value(QString::number(apa.getAdcAddr()+1));
   shortName2->setText(st1+ad.getShortName()+st2);
   if(modul->getTypeModule() == 4){
      ad = adcs->value(QString::number(apa.getAdcAddr()+2));
      shortName3->setText(st1+ad.getShortName()+st2);
      ad = adcs->value(QString::number(apa.getAdcAddr()+3));
      shortName4->setText(st1+ad.getShortName()+st2);
      ad = adcs->value(QString::number(apa.getAdcAddr()+4));
      shortName5->setText(st1+ad.getShortName()+st2);
      ad = adcs->value(QString::number(apa.getAdcAddr()+5));
      shortName6->setText(st1+ad.getShortName()+st2);
      ad = adcs->value(QString::number(apa.getAdcAddr()+6));
      shortName7->setText(st1+ad.getShortName()+st2);
      ad = adcs->value(QString::number(apa.getAdcAddr()+7));
      shortName8->setText(st1+ad.getShortName()+st2);
   }
}


void SettingsAnalog::init()
{
    QHBoxLayout *hBox = new QHBoxLayout();
    hBox->addWidget(minValACp());
    hBox->addWidget(labLayout());
    hBox->addWidget(maxValACp());
    QVBoxLayout *vBox = new QVBoxLayout();
    vBox->addLayout(hBox);
    vBox->addLayout(btLayout());
    vProgrBar = new MyProgressBar(sbar);
    //vProgrBar->progress->setOrientation(Qt::Vertical);
    vProgrBar->progress->show();
    //vProgrBar->progress->setGeometry(17,26,17,210);
    vProgrBar->progress->setValue(0);
    sbar->addWidget(vProgrBar);
    vBox->addWidget(sbar);
    setLayout(vBox);

}

//Интерфейс////////////////////////////////////////////////////////////////////////////////////////////////////////////
QGroupBox *SettingsAnalog::minValACp()
{
    QGroupBox *grBox = new QGroupBox("Min");
    QVBoxLayout *vBox = new QVBoxLayout();
    edAcp1Low = new QLineEdit("");
    edAcp1Low->setFixedWidth(60);
    edAcp1Low->setAlignment(Qt::AlignRight);
    edAcp1Low->setStyleSheet("color:blue");
    vBox->addWidget(edAcp1Low);
    edAcp2Low = new QLineEdit("");
    edAcp2Low->setFixedWidth(60);
    edAcp2Low->setAlignment(Qt::AlignRight);
    edAcp2Low->setStyleSheet("color:blue");
    vBox->addWidget(edAcp2Low);
    edAcp3Low = new QLineEdit("");
    edAcp3Low->setFixedWidth(60);
    edAcp3Low->setAlignment(Qt::AlignRight);
    edAcp3Low->setStyleSheet("color:blue");
    vBox->addWidget(edAcp3Low);
    edAcp4Low = new QLineEdit("");
    edAcp4Low->setFixedWidth(60);
    edAcp4Low->setAlignment(Qt::AlignRight);
    edAcp4Low->setStyleSheet("color:blue");
    vBox->addWidget(edAcp4Low);
    edAcp5Low = new QLineEdit("");
    edAcp5Low->setFixedWidth(60);
    edAcp5Low->setAlignment(Qt::AlignRight);
    edAcp5Low->setStyleSheet("color:blue");
    vBox->addWidget(edAcp5Low);
    edAcp6Low = new QLineEdit("");
    edAcp6Low->setFixedWidth(60);
    edAcp6Low->setAlignment(Qt::AlignRight);
    edAcp6Low->setStyleSheet("color:blue");
    vBox->addWidget(edAcp6Low);
    edAcp7Low = new QLineEdit("");
    edAcp7Low->setFixedWidth(60);
    edAcp7Low->setAlignment(Qt::AlignRight);
    edAcp7Low->setStyleSheet("color:blue");
    vBox->addWidget(edAcp7Low);
    edAcp8Low = new QLineEdit("");
    edAcp8Low->setFixedWidth(60);
    edAcp8Low->setAlignment(Qt::AlignRight);
    edAcp8Low->setStyleSheet("color:blue");
    vBox->addWidget(edAcp8Low);
    grBox->setLayout(vBox);

    return grBox;
}

QGroupBox *SettingsAnalog::maxValACp()
{
    QGroupBox *grBox = new QGroupBox("Max");
    QVBoxLayout *vBox = new QVBoxLayout();
    edAcp1Hi = new QLineEdit("");
    edAcp1Hi->setFixedWidth(60);
    edAcp1Hi->setAlignment(Qt::AlignRight);
    edAcp1Hi->setStyleSheet("color:red");
    vBox->addWidget(edAcp1Hi);
    edAcp2Hi = new QLineEdit("");
    edAcp2Hi->setFixedWidth(60);
    edAcp2Hi->setAlignment(Qt::AlignRight);
    edAcp2Hi->setStyleSheet("color:red");
    vBox->addWidget(edAcp2Hi);
    edAcp3Hi = new QLineEdit("");
    edAcp3Hi->setFixedWidth(60);
    edAcp3Hi->setAlignment(Qt::AlignRight);
    edAcp3Hi->setStyleSheet("color:red");
    vBox->addWidget(edAcp3Hi);
    edAcp4Hi = new QLineEdit("");
    edAcp4Hi->setFixedWidth(60);
    edAcp4Hi->setAlignment(Qt::AlignRight);
    edAcp4Hi->setStyleSheet("color:red");
    vBox->addWidget(edAcp4Hi);
    edAcp5Hi = new QLineEdit("");
    edAcp5Hi->setFixedWidth(60);
    edAcp5Hi->setAlignment(Qt::AlignRight);
    edAcp5Hi->setStyleSheet("color:red");
    vBox->addWidget(edAcp5Hi);
    edAcp6Hi = new QLineEdit("");
    edAcp6Hi->setFixedWidth(60);
    edAcp6Hi->setAlignment(Qt::AlignRight);
    edAcp6Hi->setStyleSheet("color:red");
    vBox->addWidget(edAcp6Hi);
    edAcp7Hi = new QLineEdit("");
    edAcp7Hi->setFixedWidth(60);
    edAcp7Hi->setAlignment(Qt::AlignRight);
    edAcp7Hi->setStyleSheet("color:red");
    vBox->addWidget(edAcp7Hi);
    edAcp8Hi = new QLineEdit("");
    edAcp8Hi->setFixedWidth(60);
    edAcp8Hi->setAlignment(Qt::AlignRight);
    edAcp8Hi->setStyleSheet("color:red");
    vBox->addWidget(edAcp8Hi);
    grBox->setLayout(vBox);

    return grBox;
}

QGroupBox *SettingsAnalog::labLayout()
{
    QGroupBox *gBox = new QGroupBox("Название уставки");
    QVBoxLayout *vBox = new QVBoxLayout();
    shortName1 = new QLabel("");
    shortName1->setFixedWidth(160);
    shortName1->setAlignment(Qt::AlignCenter);
    shortName1->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    vBox->addWidget(shortName1);
    shortName2 = new QLabel("");
    shortName2->setFixedWidth(160);
    shortName2->setAlignment(Qt::AlignCenter);
    shortName2->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    vBox->addWidget(shortName2);
    shortName3 = new QLabel("");
    shortName3->setFixedWidth(160);
    shortName3->setAlignment(Qt::AlignCenter);
    shortName3->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    vBox->addWidget(shortName3);
    shortName4 = new QLabel("");
    shortName4->setFixedWidth(160);
    shortName4->setAlignment(Qt::AlignCenter);
    shortName4->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    vBox->addWidget(shortName4);
    shortName5 = new QLabel("");
    shortName5->setFixedWidth(160);
    shortName5->setAlignment(Qt::AlignCenter);
    shortName5->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    vBox->addWidget(shortName5);
    shortName6 = new QLabel("");
    shortName6->setFixedWidth(160);
    shortName6->setAlignment(Qt::AlignCenter);
    shortName6->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    vBox->addWidget(shortName6);
    shortName7 = new QLabel("");
    shortName7->setFixedWidth(160);
    shortName7->setAlignment(Qt::AlignCenter);
    shortName7->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    vBox->addWidget(shortName7);
    shortName8 = new QLabel("");
    shortName8->setFixedWidth(160);
    shortName8->setAlignment(Qt::AlignCenter);
    shortName8->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    vBox->addWidget(shortName8);
    gBox->setLayout(vBox);

    return gBox;
}

QHBoxLayout *SettingsAnalog::btLayout()
{
    QHBoxLayout *hBox = new QHBoxLayout();
    btWrite = new QPushButton("Записать");
    btClose = new QPushButton("Выйти");
    hBox->addWidget(btWrite);
    hBox->addWidget(btClose);

    return hBox;
}

//====================================================================================================================//
