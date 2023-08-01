#include <QThread>

#include "ccmbb.h"
#include "calibratedialog.h"



CalibrateDialog::CalibrateDialog(DataModules *data, QWidget *parent, Qt::WindowFlags f):QDialog(parent, f)
{
    //setAttribute(Qt::WA_DeleteOnClose);
    MainWindow *mainWin = (MainWindow*)parent;
    sPorts = mainWin->ports;
    modul = data;
    setModal(true);
    init();
    initData();
    setWindowIcon(QIcon(":/cmbbImages/measure.png"));
    setWindowTitle("Калибровка модуля " + modul->getTypeMod()+" "+modul->getIdMod()+"//"+modul->getVersionMod());
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());

    connect(this, SIGNAL(changeProgress(int)), vProgrBar, SLOT(nextStep(int)));
    connect(btMeasur, SIGNAL(clicked()), this, SLOT(makeCalibrate()));
    //Измерения/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    connect(this, SIGNAL(sigMeasur0(int, DataModules*, bool*, uint*)),sPorts.at(0) ,SLOT(makeMeasur(int, DataModules*,bool*, uint*)));
    connect(this, SIGNAL(sigMeasur1(int, DataModules*, bool*, uint*)),sPorts.at(1) ,SLOT(makeMeasur(int, DataModules*,bool*, uint*)));
    //Kалибровкa////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    connect(btWrite, SIGNAL(clicked()), this, SLOT(writeCalibrationFactor()));
    connect(this, SIGNAL(sigWriteCalFactor0(int, int, int, int, uint*)), sPorts.at(0), SLOT(makeWriteCalFactor(int, int, int, int, uint*)));
    connect(this, SIGNAL(sigWriteCalFactor1(int, int, int, int, uint*)), sPorts.at(1), SLOT(makeWriteCalFactor(int, int, int, int, uint*)));
    connect(btReset, SIGNAL(clicked()), this, SLOT(resetCalibCoeff()));
    connect(this, SIGNAL(sigResetCalCoeff0(int,int)), sPorts.at(0), SLOT(ResetCalCoeff(int,int)));
    connect(this, SIGNAL(sigResetCalCoeff1(int,int)), sPorts.at(1), SLOT(ResetCalCoeff(int,int)));
    connect(btControl, SIGNAL(clicked()), this, SLOT(ControlMeasur()));
    connect(this, SIGNAL(sigControlMeasur0(int, int,int,uint*,LogFile*)), sPorts.at(0), SLOT(controlMeasur(int, int,int,uint*,LogFile*)));
    connect(this, SIGNAL(sigControlMeasur1(int, int,int,uint*,LogFile*)), sPorts.at(1), SLOT(controlMeasur(int, int,int,uint*,LogFile*)));
    connect(btReturn, SIGNAL(clicked()), this, SLOT(close()));
}

CalibrateDialog::~CalibrateDialog()
{

}

//Инициализация данных====================================//
void CalibrateDialog::initData()
{
    res[0] = 0;
    res[1] = 0;
    res[2] = 0;
    res[3] = 0;
    res[4] = 0;
    res[5] = 0;
    res[6] = 0;
    res[7] = 0;
    num_izm = 0;

    if(modul->getTypeModule() != 4){
        txtAcp3Aver->setDisabled(true);
        txtAcp3Devi->setDisabled(true);
        txtAcp4Aver->setDisabled(true);
        txtAcp4Devi->setDisabled(true);
        txtAcp5Aver->setDisabled(true);
        txtAcp5Devi->setDisabled(true);
        txtAcp6Aver->setDisabled(true);
        txtAcp6Devi->setDisabled(true);
        txtAcp7Aver->setDisabled(true);
        txtAcp7Devi->setDisabled(true);
        txtAcp8Aver->setDisabled(true);
        txtAcp8Devi->setDisabled(true);
        btAcp3->setDisabled(true);
        btAcp4->setDisabled(true);
        btAcp5->setDisabled(true);
        btAcp6->setDisabled(true);
        btAcp7->setDisabled(true);
        btAcp8->setDisabled(true);
        txtAcp3->setDisabled(true);
        txtAcp4->setDisabled(true);
        txtAcp5->setDisabled(true);
        txtAcp6->setDisabled(true);
        txtAcp7->setDisabled(true);
        txtAcp8->setDisabled(true);
        txtAcp3Aver->setText("0");
        txtAcp3Devi->setText("0");
        txtAcp4Aver->setText("0");
        txtAcp4Devi->setText("0");
        txtAcp5Aver->setText("0");
        txtAcp5Devi->setText("0");
        txtAcp6Aver->setText("0");
        txtAcp6Devi->setText("0");
        txtAcp7Aver->setText("0");
        txtAcp7Devi->setText("0");
        txtAcp8Aver->setText("0");
        txtAcp8Devi->setText("0");
        txtAcp3->setText("0");
        txtAcp4->setText("0");
        txtAcp5->setText("0");
        txtAcp6->setText("0");
        txtAcp7->setText("0");
        txtAcp8->setText("0");
        res[3] = 0;
        res[4] = 0;
        res[5] = 0;
        res[6] = 0;
        res[7] = 0;
    }
}

//Контрольные измерения============================================================================//
void CalibrateDialog::ControlMeasur()
{
    LogFile *logFile=nullptr;

    int type = modul->getTypeModule();
    int numCom = modul->getNumLine().toInt()-1;//Номер линии//
    int addr = modul->getAddrDecMod().toInt();
    int numLine = modul->getNumInternal().toInt();
    int func = numLine;

    disabledButton(true);
    repaint();
    QThread::msleep(100);

    if(chSlave->isChecked()){
       func = numLine==1?2:1;
    }

    if(chWriteFile->isChecked()){
        QDir d;
        QString path = d.currentPath()+QDir::separator()+"log"+QDir::separator()+"CMBB_CONTROL.log";
        logFile = new LogFile(path);
        logFile->appendString("Контрольные измерения для модуля " + modul->getTypeMod()+" ID:"+modul->getIdMod()+" ver:"+modul->getVersionMod());
        if(type != 4){
           logFile->appendString("АЦП-1\tАЦП-2\t");
        }else{
           logFile->appendString("АЦП-1\tАЦП-2\tАЦП-3\tАЦП-4\tАЦП-5\tАЦП-6\tАЦП-7\tАЦП-8\t");
        }
    }

    for(int i=0; i < 30; i++){
        switch(numCom){
        case 0:
             emit sigControlMeasur0(func, type, addr, cIzm, logFile);
            break;
        case 1:
             emit sigControlMeasur1(func, type, addr, cIzm, logFile);
            break;
        }
        fillControl(type);
        QThread::usleep(2*100000);
    }
    disabledButton(false);
    btReturn->setFocus();
    if(logFile){
       delete logFile;
    }
}
//=================================================================================================//


//Калибровка/////////////////////////////////////////////////////////////////////////////////////////
void CalibrateDialog::makeCalibrate()
{
    vProgrBar->progress->setValue(0);
    QThread::usleep(cntPeriod->value()*1000);
    int type = modul->getTypeModule();
    num_izm = 0;
    disabledButton(true);
    flOK = false;
    int numCom = modul->getNumLine().toInt()-1;//Номер линии//

    for(int i=0; i < cntTotalMeasur->value(); i++){
        switch(numCom){
        case 0:
             emit sigMeasur0(i, modul, &flOK, izm[0]);
            break;
        case 1:
             emit sigMeasur1(i, modul, &flOK, izm[0]);
            break;
        }
        if(flOK){
            num_izm++;
        }
        emit changeProgress(i*100/cntTotalMeasur->value());
        QThread::usleep(cntPeriod->value()*1000);
    }

    processMeasur(type);
    fillFIelds(type);
    disabledButton(false);
    btReturn->setFocus();
    vProgrBar->progress->setValue(100);
}

void CalibrateDialog::processMeasur(int type)
{
    uint acpCnt = 8;

    if(type != 4){
       acpCnt = 2;
    }

    for(uint i = 0;i < acpCnt; i++)//Расчет////////////////////////
    {
          res[i] = 0;
          for(uint j = 0; j < num_izm; j++)
            res[i] += izm[j][i];
          res[i] = res[i]/num_izm;
          srotkl[i] = 0;
          for(uint j = 0; j < num_izm; j++)
            srotkl[i] += (izm[j][i] - res[i])*(izm[j][i] - res[i]);
          srotkl[i] = srotkl[i]/num_izm;
    }
}

void CalibrateDialog::disabledButton(bool flag)
{
    btControl->setDisabled(flag);
    btMeasur->setDisabled(flag);
    btReset->setDisabled(flag);
    btReturn->setDisabled(flag);
    //btStop->setDisabled(flag);
    btWrite->setDisabled(flag);
}

void CalibrateDialog::fillFIelds(int type)
{
    QString s;
    txtAcp1Aver->setText(s.sprintf("%2.3f",res[0] * 2.0 /1000.0));
    txtAcp1Devi->setText(s.sprintf("%2.3f",srotkl[0] * 2.0 /1000.0));
    txtAcp2Aver->setText(s.sprintf("%2.3f",res[1] * 2.0 /1000.0));
    txtAcp2Devi->setText(s.sprintf("%2.3f",srotkl[1] * 2.0 /1000.0));

    if(type ==4){
        txtAcp3Aver->setText(s.sprintf("%2.3f",res[2] * 2.0 /1000.0));
        txtAcp3Devi->setText(s.sprintf("%2.3f",srotkl[2] * 2.0 /1000.0));
        txtAcp4Aver->setText(s.sprintf("%2.3f",res[3] * 2.0 /1000.0));
        txtAcp4Devi->setText(s.sprintf("%2.3f",srotkl[3] * 2.0 /1000.0));
        txtAcp5Aver->setText(s.sprintf("%2.3f",res[4] * 2.0 /1000.0));
        txtAcp5Devi->setText(s.sprintf("%2.3f",srotkl[4] * 2.0 /1000.0));
        txtAcp6Aver->setText(s.sprintf("%2.3f",res[5] * 2.0 /1000.0));
        txtAcp6Devi->setText(s.sprintf("%2.3f",srotkl[5] * 2.0 /1000.0));
        txtAcp7Aver->setText(s.sprintf("%2.3f",res[6] * 2.0 /1000.0));
        txtAcp7Devi->setText(s.sprintf("%2.3f",srotkl[6] * 2.0 /1000.0));
        txtAcp8Aver->setText(s.sprintf("%2.3f",res[7] * 2.0 /1000.0));
        txtAcp8Devi->setText(s.sprintf("%2.3f",srotkl[7] * 2.0 /1000.0));
    }
}//=========================================================================================//

void CalibrateDialog::writeCalibrationFactor()
{
    if (num_izm == 0)
       QMessageBox::information(this, "Сообщение", "Необходимо произвести измерения!");
     else
     {
        disabledButton(true);
        repaint();
        int type = modul->getTypeModule();
        int numCom = modul->getNumLine().toInt()-1;//Номер линии//
        int idxMa = btMa4->isChecked()?0:1;
        int numAcp = getNumAcp();
        int addr = modul->getAddrDecMod().toInt();

        switch(numCom){
        case 0:
             emit sigWriteCalFactor0(type, idxMa, numAcp, addr, res);
            break;
        case 1:
             emit sigWriteCalFactor1(type, idxMa, numAcp, addr, res);
            break;
        }
        disabledButton(false);
        btReturn->setFocus();
    }
}

void CalibrateDialog::resetCalibCoeff()
{
    int type = modul->getTypeModule();
    int numCom = modul->getNumLine().toInt()-1;//Номер линии//
    int addr = modul->getAddrDecMod().toInt();

    switch(numCom){
    case 0:
         emit sigResetCalCoeff0(type, addr);
        break;
    case 1:
         emit sigResetCalCoeff1(type, addr);
        break;
    }
    btReturn->setFocus();
}

int CalibrateDialog::getNumAcp()
{
    int retVal = -1;

    if(btAcp1->isChecked())
       retVal = 0;
    else if(btAcp2->isChecked())
        retVal = 1;
    else if(btAcp3->isChecked())
        retVal = 2;
    else if(btAcp4->isChecked())
        retVal = 3;
    else if(btAcp5->isChecked())
        retVal = 4;
    else if(btAcp6->isChecked())
        retVal = 5;
    else if(btAcp7->isChecked())
        retVal = 6;
    else if(btAcp8->isChecked())
        retVal = 7;

    return retVal;
}

void CalibrateDialog::fillControl(int type)
{
    txtAcp1->setText(QString::number(cIzm[0]));
    txtAcp2->setText(QString::number(cIzm[1]));
    if(type == 4){
       txtAcp3->setText(QString::number(cIzm[2]));
       txtAcp4->setText(QString::number(cIzm[3]));
       txtAcp5->setText(QString::number(cIzm[4]));
       txtAcp6->setText(QString::number(cIzm[5]));
       txtAcp7->setText(QString::number(cIzm[6]));
       txtAcp8->setText(QString::number(cIzm[7]));
    }
}

//==========================================================================================//
//==========================================================================================//
//Интерфейс формы/////////////////////////////////////////////////////////////////////////////
void CalibrateDialog::init()
{
    btReturn = new QPushButton("Возврат");
    QHBoxLayout *hlAcp = new QHBoxLayout();
    hlAcp->addWidget(createLayoutPgrogressBar());
    hlAcp->addWidget(createGroupAcp1());
    hlAcp->addWidget(createGroupAcp2());
    hlAcp->addLayout(createHLayoutSpinCount());

    QHBoxLayout *hlKalibr = new QHBoxLayout();
    hlKalibr->addWidget(createGroupKalibr());
    hlKalibr->addWidget(createGroupKanal());
    hlKalibr->addLayout(createGroupBT());

    QHBoxLayout *hlLoad = new QHBoxLayout();
    hlLoad->addLayout(createGroupACP1_4());
    hlLoad->addLayout(createGroupACP5_8());
    hlLoad->addLayout(createGroupLoad());

    QVBoxLayout *vb = new QVBoxLayout();
    vb->addLayout(hlAcp);
    vb->addLayout(hlKalibr);
    vb->addLayout(hlLoad);
    vb->addWidget(btReturn);

    setLayout(vb);
}

//===Layout:Выполнение калибровки=================================//
QGroupBox *CalibrateDialog::createLayoutPgrogressBar()
{
    QGroupBox *gBox = new QGroupBox("...");
    QVBoxLayout *vlProgBar = new QVBoxLayout();
    vProgrBar = new MyProgressBar(this);
    vProgrBar->progress->setOrientation(Qt::Vertical);
    vProgrBar->progress->show();
    vProgrBar->progress->setGeometry(17,26,17,210);
    vProgrBar->progress->setValue(0);
    vlProgBar->addWidget(vProgrBar,0,Qt::AlignBaseline);
    gBox->setLayout(vlProgBar);

    return gBox;
}

QFormLayout *CalibrateDialog::createHLayoutSpinCount()
{
    QFormLayout *form = new QFormLayout();
    QLabel *lab1 = new QLabel("\nВсего измерений:");
    cntTotalMeasur = new QSpinBox();
    cntTotalMeasur->setValue(30);
    cntTotalMeasur->setFixedWidth(88);
    QLabel *lab2 = new QLabel("\nПериод, мс:");
    cntPeriod = new QSpinBox();
    cntPeriod->setMaximum(1000);
    cntPeriod->setValue(100);
    cntPeriod->setFixedWidth(88);
    form->addRow(lab1);
    form->addRow(cntTotalMeasur);
    form->addRow(lab2);
    form->addRow(cntPeriod);
    QLabel *labN = new QLabel("\n\n");
    form->addRow(labN);
    btMeasur = new QPushButton("Измерить");
    form->addRow(btMeasur);

    return form;
}

QGroupBox *CalibrateDialog::createGroupAcp1()
{
    QGroupBox *gBox = new QGroupBox("Cp.значение, Ма");
    QFormLayout *vlAcpAver = new QFormLayout();
    vlAcpAver->setFormAlignment(Qt::AlignHCenter);
    txtAcp1Aver = new QLineEdit("0");
    txtAcp1Aver->setFixedWidth(60);
    txtAcp1Aver->setReadOnly(true);
    vlAcpAver->addRow("АЦП1:", txtAcp1Aver);
    txtAcp2Aver = new QLineEdit("0");
    txtAcp2Aver->setFixedWidth(60);
    txtAcp2Aver->setReadOnly(true);
    vlAcpAver->addRow("АЦП2:", txtAcp2Aver);
    txtAcp3Aver = new QLineEdit("0");
    txtAcp3Aver->setFixedWidth(60);
    txtAcp3Aver->setReadOnly(true);
    vlAcpAver->addRow("АЦП3:", txtAcp3Aver);
    txtAcp4Aver = new QLineEdit("0");
    txtAcp4Aver->setFixedWidth(60);
    txtAcp4Aver->setReadOnly(true);
    vlAcpAver->addRow("АЦП4:", txtAcp4Aver);
    txtAcp5Aver = new QLineEdit("0");
    txtAcp5Aver->setFixedWidth(60);
    txtAcp5Aver->setReadOnly(true);
    vlAcpAver->addRow("АЦП5:", txtAcp5Aver);
    txtAcp6Aver = new QLineEdit("0");
    txtAcp6Aver->setFixedWidth(60);
    txtAcp6Aver->setReadOnly(true);
    vlAcpAver->addRow("АЦП6:", txtAcp6Aver);
    txtAcp7Aver = new QLineEdit("0");
    txtAcp7Aver->setFixedWidth(60);
    txtAcp7Aver->setReadOnly(true);
    vlAcpAver->addRow("АЦП7:", txtAcp7Aver);
    txtAcp8Aver = new QLineEdit("0");
    txtAcp8Aver->setFixedWidth(60);
    txtAcp8Aver->setReadOnly(true);
    vlAcpAver->addRow("АЦП8:", txtAcp8Aver);
    gBox->setLayout(vlAcpAver);

    return gBox;
}


QGroupBox *CalibrateDialog::createGroupAcp2()
{
    QGroupBox *gBox = new QGroupBox("Ср. квадр. откл., Ма");
    QFormLayout *vlAcpDevi = new QFormLayout();
    vlAcpDevi->setFormAlignment(Qt::AlignHCenter);
    txtAcp1Devi = new QLineEdit("0");
    txtAcp1Devi->setFixedWidth(60);
    txtAcp1Devi->setReadOnly(true);
    vlAcpDevi->addRow("АЦП1:", txtAcp1Devi);
    txtAcp2Devi = new QLineEdit("0");
    txtAcp2Devi->setFixedWidth(60);
    txtAcp2Devi->setReadOnly(true);
    vlAcpDevi->addRow("АЦП2:", txtAcp2Devi);
    txtAcp3Devi = new QLineEdit("0");
    txtAcp3Devi->setFixedWidth(60);
    txtAcp3Devi->setReadOnly(true);
    vlAcpDevi->addRow("АЦП3:", txtAcp3Devi);
    txtAcp4Devi = new QLineEdit("0");
    txtAcp4Devi->setFixedWidth(60);
    txtAcp4Devi->setReadOnly(true);
    vlAcpDevi->addRow("АЦП4:", txtAcp4Devi);
    txtAcp5Devi = new QLineEdit("0");
    txtAcp5Devi->setFixedWidth(60);
    txtAcp5Devi->setReadOnly(true);
    vlAcpDevi->addRow("АЦП5:", txtAcp5Devi);
    txtAcp6Devi = new QLineEdit("0");
    txtAcp6Devi->setFixedWidth(60);
    txtAcp6Devi->setReadOnly(true);
    vlAcpDevi->addRow("АЦП6:", txtAcp6Devi);
    txtAcp7Devi = new QLineEdit("0");
    txtAcp7Devi->setFixedWidth(60);
    txtAcp7Devi->setReadOnly(true);
    vlAcpDevi->addRow("АЦП7:", txtAcp7Devi);
    txtAcp8Devi = new QLineEdit("0");
    txtAcp8Devi->setFixedWidth(60);
    txtAcp8Devi->setReadOnly(true);
    vlAcpDevi->addRow("АЦП8:", txtAcp8Devi);
    gBox->setLayout(vlAcpDevi);

    return gBox;
}
//=========================================================//

//Layout:Калибрация и Канал=======================================//
QGroupBox *CalibrateDialog::createGroupKalibr()
{
    QGroupBox *groupBox = new QGroupBox(tr("Калибровка"));
    btMa4  = new QRadioButton("4 Ma");
    btMa20 = new QRadioButton("20 Ma");
    btMa4->setChecked(true);
    QVBoxLayout *vBox    = new QVBoxLayout();
    vBox->addWidget(btMa4);
    vBox->addWidget(btMa20);
    groupBox->setLayout(vBox);

    return groupBox;
}

QGroupBox *CalibrateDialog::createGroupKanal()
{
    QGroupBox *groupBox = new QGroupBox(tr("Канал"));
    btAcp1  = new QRadioButton("АЦП1");
    btAcp2  = new QRadioButton("АЦП2");
    btAcp3  = new QRadioButton("АЦП3");
    btAcp4  = new QRadioButton("АЦП4");
    btAcp5  = new QRadioButton("АЦП5");
    btAcp6  = new QRadioButton("АЦП6");
    btAcp7  = new QRadioButton("АЦП7");
    btAcp8  = new QRadioButton("АЦП8");
    btAcp1->setChecked(true);
    QVBoxLayout *vBox    = new QVBoxLayout();
    vBox->addWidget(btAcp1);
    vBox->addWidget(btAcp2);
    vBox->addWidget(btAcp3);
    vBox->addWidget(btAcp4);
    vBox->addWidget(btAcp5);
    vBox->addWidget(btAcp6);
    vBox->addWidget(btAcp7);
    vBox->addWidget(btAcp8);
    groupBox->setLayout(vBox);

    return groupBox;
}

QVBoxLayout *CalibrateDialog::createGroupBT()
{
    btWrite = new QPushButton("Записать");
    btReset = new QPushButton("Сбросить");
    QVBoxLayout *vBox = new QVBoxLayout();
    vBox->addWidget(btWrite);
    vBox->addWidget(btReset);

    return vBox;
}
//=========================================================//

//==Layout:Сохранение=============================================//
QFormLayout *CalibrateDialog::createGroupACP1_4()
{
    QFormLayout *form = new QFormLayout();
    QLabel *lab1 = new QLabel("АЦП1:");
    txtAcp1 = new QLineEdit("0");
    txtAcp1->setFixedWidth(60);
    txtAcp1->setReadOnly(true);
    QLabel *lab2 = new QLabel("АЦП2:");
    txtAcp2 = new QLineEdit("0");
    txtAcp2->setFixedWidth(60);
    txtAcp2->setReadOnly(true);
    QLabel *lab3 = new QLabel("АЦП3:");
    txtAcp3 = new QLineEdit("0");
    txtAcp3->setFixedWidth(60);
    txtAcp3->setReadOnly(true);
    QLabel *lab4 = new QLabel("АЦП4:");
    txtAcp4 = new QLineEdit("0");
    txtAcp4->setFixedWidth(60);
    txtAcp4->setReadOnly(true);
    form->addRow(lab1,txtAcp1);
    form->addRow(lab2,txtAcp2);
    form->addRow(lab3,txtAcp3);
    form->addRow(lab4,txtAcp4);

    return form;
}

QFormLayout *CalibrateDialog::createGroupACP5_8()
{
    QFormLayout *form = new QFormLayout();
    QLabel *lab1 = new QLabel("АЦП5:");
    txtAcp5 = new QLineEdit("0");
    txtAcp5->setFixedWidth(60);
    txtAcp5->setReadOnly(true);
    QLabel *lab2 = new QLabel("АЦП6:");
    txtAcp6 = new QLineEdit("0");
    txtAcp6->setFixedWidth(60);
    txtAcp6->setReadOnly(true);
    QLabel *lab3 = new QLabel("АЦП7:");
    txtAcp7 = new QLineEdit("0");
    txtAcp7->setFixedWidth(60);
    txtAcp7->setReadOnly(true);
    QLabel *lab4 = new QLabel("АЦП8:");
    txtAcp8 = new QLineEdit("0");
    txtAcp8->setFixedWidth(60);
    txtAcp8->setReadOnly(true);
    form->addRow(lab1,txtAcp5);
    form->addRow(lab2,txtAcp6);
    form->addRow(lab3,txtAcp7);
    form->addRow(lab4,txtAcp8);

    return form;

}

QVBoxLayout *CalibrateDialog::createGroupLoad()
{
    QVBoxLayout *vBox = new QVBoxLayout();
    chWriteFile = new QCheckBox("Запись в файл");
    chSlave     = new QCheckBox("Slave");
    btControl   = new QPushButton("Контроль");
    //btStop     = new QPushButton("Стоп");
    btReset->setFixedWidth(90);
    vBox->addWidget(chWriteFile);
    vBox->addWidget(chSlave);
    vBox->addWidget(btControl);
    //vBox->addWidget(btStop);

    return vBox;
}

//=============================================================================================================//
//=============================================================================================================//

