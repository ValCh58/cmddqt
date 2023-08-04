#include "calibrationcheck.h"

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStatusBar>
#include <QSpinBox>

#include <thread>
#include <chrono>

#include "ccmbb.h"
#include "sport.h"
#include "qdebug.h"


/**
 * @brief CalibrationCheck::CalibrationCheck
 * @param data
 * @param parent
 * @param f
 */
CalibrationCheck::CalibrationCheck(SPort *port, DataModules *data, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    setWindowTitle("Проверка вычисленных калибровочных коэффициентов и межмодульного обмена");
    setWindowIcon(QIcon(":/cmbbImages/mbb1.png"));
    sport = port;
    modul = data;

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(initCheck1(),0,0);
    grid->addWidget(initCheck2(),0,1);
    grid->addWidget(initBtnForm(), 2, 1);
    setLayout(grid);
}

/**
 * @brief CalibrationCheck::~CalibrationCheck
 */
CalibrationCheck::~CalibrationCheck(){

}

/**
 * @brief CalibrationMBAC::setFormatLineEdit
 * @param qle
 * @param nmfont
 * @param size
 * @param bold
 * @param align
 * @param len
 * @param width
 * @param clbtn
 */
void CalibrationCheck::setFormatLineEdit(QLineEdit *qle, QString nmfont, int size,
                                        int bold, Qt::Alignment align, int len, int width, bool clbtn){
     QFont serifFont(nmfont, size, bold);
     qle->setAlignment(align);
     qle->setClearButtonEnabled(clbtn);
     qle->setFont(serifFont);
     qle->setMaxLength(len);
     qle->setFixedWidth(width);
    }

/**
 * @brief CalibrationMBAC::setFormatSpinBox
 * @param sb
 * @param bold
 * @param min
 * @param max
 */
void CalibrationCheck::setFormatSpinBox(QSpinBox *sb, int bold, int min, int max){
     QFont serifFont("Times", 10, bold);
     sb->setMaximum(max);
     sb->setMinimum(min);
     sb->setValue(0);
     sb->setFont(serifFont);
}

/**
 * Создание диалогового сообщения *
 * @brief CalibrationMBAC::showDialogInform
 * @param iconType
 * @param qstr
 */
void CalibrationCheck::showDialogInform(QMessageBox::Icon iconType, QString qstr){
    QMessageBox qMsg;
    qMsg.setIcon(iconType);
    qMsg.setText(qstr);
    qMsg.exec();
}

/**
 * @brief CalibrationCheck::initCheck1
 * @return
 */
QGroupBox *CalibrationCheck::initCheck1()
{
    QHBoxLayout *h1Box = new QHBoxLayout();
    labCalibrKo1 = new QLabel("ЦАП-1 задание\nкалибровки");
    spinBox1 = new QSpinBox(this);
    setFormatSpinBox(spinBox1, QFont::Normal, -99999, 99999);
    buttonOk1 = new QPushButton("Чтение АЦП-1");
    buttonOk1->setAutoDefault(false);
    connect(buttonOk1, &QPushButton::pressed, this, &CalibrationCheck::setButtonReadADC1);

    //QDoubleValidator *dv = new QDoubleValidator(0.0, 99.0, 3);
    //dv->setNotation(QDoubleValidator::StandardNotation);
    labAdcCalibr1 = new QLabel("АЦП-1 калибров.");
    editAdcCalibr1 = new QLineEdit();
    //editAdcCalibr1->setValidator(dv);
    setFormatLineEdit(editAdcCalibr1, "Times", 10, QFont::DemiBold, Qt::AlignCenter, 10, 140, false);
    editAdcCalibr1->setReadOnly(true);
    editAdcCalibr1->setStyleSheet("QLineEdit {background-color: #F5F6CE;}");

    labAdcDev1 = new QLabel("Погрешность, %");
    editAdcDev1 = new QLineEdit();
    editAdcDev1->setReadOnly(true);

    //editAdcDev1->setValidator(dv);
    setFormatLineEdit(editAdcDev1, "Times", 10, QFont::Normal, Qt::AlignCenter, 10, 140, false);
    editAdcDev1->setStyleSheet("QLineEdit {background-color: #F79F81;}");
    labAdcMa1 = new QLabel("Ток, ма");
    editAdcMa1 = new QLineEdit();
    //editAdcMa1->setValidator(dv);
    setFormatLineEdit(editAdcMa1, "Times", 10, QFont::DemiBold, Qt::AlignCenter, 10, 140, true);

    labAdcMaDev1 = new QLabel("Погрешность, %");
    editAdcMaDev1 = new QLineEdit();
    editAdcMaDev1->setReadOnly(true);
    editAdcMaDev1->setStyleSheet("QLineEdit {background-color: #F79F81;}");
    //editAdcMaDev1->setValidator(dv);
    setFormatLineEdit(editAdcMaDev1, "Times", 10, QFont::QFont::Normal, Qt::AlignCenter, 10, 140, false);

    buttonCalc1 = new QPushButton("Расчет");
    buttonCalc1->setAutoDefault(false);
    connect(buttonCalc1, &QPushButton::pressed, this, &CalibrationCheck::getButtonCalc1);

    h1Box->addWidget(createGroupRadioBtnCheck1());
    h1Box->addWidget(makeGroupBoxCheck1(""));
    QGroupBox *grb = new QGroupBox(tr("ЦАП-1/АЦП-1"));
    grb->setLayout(h1Box);

    return grb;
}

/**
 * @brief CalibrationCheck::makeGroupBoxCheck1
 * @param name
 * @return
 */
QGroupBox *CalibrationCheck::makeGroupBoxCheck1(QString name)
{
      QGroupBox *gBox = new QGroupBox(name);
      QFormLayout *formLay = new QFormLayout();

      formLay->addRow(labCalibrKo1);
      formLay->addRow(spinBox1, buttonOk1);
      formLay->addRow(labAdcCalibr1, labAdcDev1);
      formLay->addRow(editAdcCalibr1, editAdcDev1);
      formLay->addRow(labAdcMa1, labAdcMaDev1);
      formLay->addRow(editAdcMa1, editAdcMaDev1);
      formLay->insertRow(6, buttonCalc1);

      gBox->setLayout(formLay);
      return gBox;
}

/**
 * @brief CalibrationCheck::createGroupRadioBtnCheck1
 * @return
 */
QGroupBox *CalibrationCheck:: createGroupRadioBtnCheck1()
{
    QGroupBox *groupBox = new QGroupBox(tr("Ток, мА"));
    btnGroupCheck1 = new QButtonGroup(this);

    QRadioButton *radio1_1ma  = new QRadioButton(tr("1 mA"));
    btnGroupCheck1->addButton(radio1_1ma,   1102);
    QRadioButton *radio2_4ma  = new QRadioButton(tr("4 mA"));
    btnGroupCheck1->addButton(radio2_4ma,   1204);
    QRadioButton *radio3_8ma  = new QRadioButton(tr("8 mA"));
    btnGroupCheck1->addButton(radio3_8ma,   1308);
    QRadioButton *radio4_12ma = new QRadioButton(tr("12 mA"));
    btnGroupCheck1->addButton(radio4_12ma,  1412);
    QRadioButton *radio5_16ma = new QRadioButton(tr("16 mA"));
    btnGroupCheck1->addButton(radio5_16ma,  1516);
    QRadioButton *radio6_20ma = new QRadioButton(tr("20 mA"));
    btnGroupCheck1->addButton(radio6_20ma,  1620);

    connect(btnGroupCheck1, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &CalibrationCheck::getClick);


    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1_1ma);
    vbox->addWidget(radio2_4ma);
    vbox->addWidget(radio3_8ma);
    vbox->addWidget(radio4_12ma);
    vbox->addWidget(radio5_16ma);
    vbox->addWidget(radio6_20ma);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

  return groupBox;
}

/**
 * Slot *
 * @brief CalibrationCheck::getClick
 * @param id
 */
void CalibrationCheck::getClick(int id){
    idRadioBtn = id;
    int groupBtn = 0, numberBtn = 0, maBtn=0;
    getRadioBtnPush(idRadioBtn, &groupBtn, &numberBtn, &maBtn);
    switch(groupBtn){
    case GROUP_RADIO_BTN_DAC1:
    spinBox1->setValue(getGivenValue(maBtn));
    clearText(editAdcCalibr1, editAdcDev1, editAdcMa1, editAdcMaDev1);
    if(btnGroupCheck1->button(id)->isChecked()){
        btnGroupCheck1->button(id)->setIcon(QIcon(""));
        buttonOk1->setDefault(true);
    }
        break;
    case GROUP_RADIO_BTN_DAC2:
    spinBox2->setValue(getGivenValue(maBtn));
    clearText(editAdcCalibr2, editAdcDev2, editAdcMa2, editAdcMaDev2);
    if(btnGroupCheck2->button(id)->isChecked()){
        btnGroupCheck2->button(id)->setIcon(QIcon(""));
        buttonOk2->setDefault(true);
    }
        break;
    }
}

void CalibrationCheck::clearText(QLineEdit *txt1, QLineEdit *txt2, QLineEdit *txt3, QLineEdit *txt4){
    txt1->clear(); txt2->clear(); txt3->clear(); txt4->clear();
}

/**
 * Slot *
 * @brief CalibrationCheck::setButtonReadADC
 */
void CalibrationCheck::setButtonReadADC1(){
    setButtonOk1();
    editAdcMa1->setFocus();
    buttonCalc1->setDefault(true);
}

/**
 * @brief CalibrationCheck::setButtonOk1
 */
void CalibrationCheck::setButtonOk1()
{
    quint8 commandAdc = 0;
    int numCom = modul->getNumLine().toInt();
    int id = idRadioBtn;
    int groupBtn = 0, numberBtn = 0, maBtn=0;
    getRadioBtnPush(id, &groupBtn, &numberBtn, &maBtn);
    bool Ok = false;
    if(groupBtn == GROUP_RADIO_BTN_DAC1){
        if(btnGroupCheck1->button(id)->isChecked()){
           btnGroupCheck1->button(id)->setIcon(QIcon(""));
        }
    }
    commandAdc = (numCom) == 1 ? 0x41 : 0x81;
    Ok = sendCommandDac12(sport, modul, commandAdc, getGivenValue(maBtn), getGivenValue(maBtn));
    if(Ok){
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            Ok = readCalibratedADCval(sport, modul, 0x4E);
    }
    if(Ok){
        //showDialogInform(QMessageBox::Information, "Команда \"Чтение калиброванного значения АЦП\" выполненo.")
                ;}
    else if(!Ok){showDialogInform(QMessageBox::Warning, "Команда \"Чтение калиброванного значения АЦП\" не выполненo.");}
    }




/**
 * Slot *
 * @brief CalibrationCheck::setButtonReadADC2
 */
void CalibrationCheck::setButtonReadADC2(){
    setButtonOk2();
    editAdcMa2->setFocus();
    buttonCalc2->setDefault(true);
}

/**
 * @brief CalibrationCheck::setButtonOk2
 */
void CalibrationCheck::setButtonOk2()
{
    int id = idRadioBtn;
    int groupBtn = 0, numberBtn = 0, maBtn=0;
    getRadioBtnPush(id, &groupBtn, &numberBtn, &maBtn);
    quint8 commandAdc = 0;
    int numCom = modul->getNumLine().toInt();
    bool Ok = false;
    if(groupBtn == GROUP_RADIO_BTN_DAC2){
        if(btnGroupCheck2->button(id)->isChecked()){
           btnGroupCheck2->button(id)->setIcon(QIcon(""));
        }
    }
    commandAdc = (numCom) == 1 ? 0x41 : 0x81;
    Ok = sendCommandDac12(sport, modul, commandAdc, getGivenValue(maBtn), getGivenValue(maBtn));
    if(Ok){
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            Ok = readCalibratedADCval(sport, modul, 0x4E);
    }
    if(Ok){
        //showDialogInform(QMessageBox::Information, "Команда \"Чтение калиброванного значения АЦП\" выполненo.")
                ;}
    else if(!Ok){showDialogInform(QMessageBox::Warning, "Команда \"Чтение калиброванного значения АЦП\" не выполненo.");}
    }

/**
 * Slot *
 * @brief CalibrationCheck::getButtonCalc1
 */
void CalibrationCheck::getButtonCalc1(){
    getButtonCalc(editAdcMa1, editAdcCalibr1, editAdcDev1, editAdcMaDev1, 1);
    btnGroupCheck1->button(idRadioBtn)->setIcon(QIcon(":/cmbbImages/accept.png"));
}

/**
 * Slot *
 * @brief CalibrationCheck::getButtonCalc2
 */
void CalibrationCheck::getButtonCalc2(){
    getButtonCalc(editAdcMa2, editAdcCalibr2, editAdcDev2, editAdcMaDev2, 2);
    btnGroupCheck2->button(idRadioBtn)->setIcon(QIcon(":/cmbbImages/accept.png"));
}

/**
 * @brief CalibrationCheck::getButtonCalc
 * @param editAdcMa
 * @param editAdcCalibr
 * @param editAdcDev
 * @param editAdcMaDev
 */
void CalibrationCheck::getButtonCalc(QLineEdit *editAdcMa, QLineEdit *editAdcCalibr, QLineEdit *editAdcDev, QLineEdit *editAdcMaDev, int numGroup){
    int id = idRadioBtn;
    int groupBtn = 0, numberBtn = 0, maBtn=0;
    getRadioBtnPush(id, &groupBtn, &numberBtn, &maBtn);
    if(groupBtn != numGroup){return;}

    editAdcMa->setText(editAdcMa->text().replace(',', '.'));
    double Device = editAdcMa->text().toDouble();//Ток от прибора//
    double Adc = editAdcCalibr->text().toDouble();//АЦП калиброванное
    double Ma = getMa(maBtn);//ЦАП ма

    double AdcDev = getDeviation(Device, Adc, 20);//%ацп
    editAdcDev->setText(QString().setNum(AdcDev, 'g', 3));//АЦП
    double DacDev = getDeviation(Ma, Device, 20); //%цап
    editAdcMaDev->setText(QString().setNum(DacDev, 'g', 3));//ЦАП
}


/**
 * Slot *
 * @brief CalibrationCheck::execCheck
 */
void CalibrationCheck::execCheck(){
   QString ms = getMessageFromAnsver(sport, modul);
   showDialogInform(QMessageBox::Information, ms);
}

/**
 * @brief CalibrationCheck::getDeviation
 * @param dac
 * @param device
 * @param ma
 * @return
 */
double CalibrationCheck::getDeviation(double dac, double device, double ma){
    return (((dac - device)/ma)*100);
}

/**
 * Запись калиброванного значения в ЦАПы *
 * @brief CalibrationMBAC::sendCommandDac12
 * @param sPort
 * @param modul
 * @param command
 * @param datLoHi01
 * @param datLoHi23
 * @return
 */
bool CalibrationCheck::sendCommandDac12(SPort *sPort,
                                       DataModules* modul,
                                       quint8 command,
                                       quint16 datLoHi01,
                                       quint16 datLoHi23){
    bool retOk = false, ok;
    quint8 lo0 = (datLoHi01 & 0x00FF);
    quint8 hi1 = (datLoHi01 & 0xFF00)>>8;
    quint8 lo2 = (datLoHi23 & 0x00FF);
    quint8 hi3 = (datLoHi23 & 0xFF00)>>8;
    quint8 adrMod = static_cast<quint8>(modul->getAddrDecMod().toUInt(&ok, 10));
    retOk = sPort->writeDataAcp12MBAC(adrMod, command, lo0, hi1, lo2, hi3);
    if(retOk) {retOk = sPort->getDataFromPortMBAC(adrMod, 3);}
    return retOk;

}

/**
 * @brief CalibrationCheck::getMa
 * @param selector
 * @return
 */
double CalibrationCheck::getMa(const int selector){
double retVal = 0;
switch(selector){
 case 2://Ma
     retVal = 1.0;
     break;
 case 4://Ma
     retVal = 4.0;
     break;
 case 8://Ma
     retVal = 8.0;
     break;
 case 12://Ma
     retVal = 12.0;
     break;
 case 16://Ma
     retVal = 16.0;
     break;
 case 20://Ma
     retVal = 20.0;
     break;
 }
 return retVal;
}

/**
 * @brief CalibrationCheck::getGivenValue
 * @param selector
 * @return
 */
   quint16 CalibrationCheck::getGivenValue(const int selector){
   quint16 retVal = 0;
   switch(selector){
    case 2://Ma
        retVal = 200;
        break;
    case 4://Ma
        retVal = 800;
        break;
    case 8://Ma
        retVal = 1600;
        break;
    case 12://Ma
        retVal = 2400;
        break;
    case 16://Ma
        retVal = 3200;
        break;
    case 20://Ma
        retVal = 4000;
        break;
    }
    return retVal;
}

/**
 * @brief CalibrationCheck::getRadioBtnPush
 * @param idBtn
 * @param group
 * @param number
 * @param ma
 */
void CalibrationCheck::getRadioBtnPush(int idBtn, int *group, int *number, int *ma)
{
    int digits [4], cnt = 0;
    while(cnt < 4){
        digits[cnt] = idBtn%10;
        idBtn /= 10;
        cnt++;}
    *group = digits[3];
    *number = digits[2];
    *ma = (QStringLiteral("%1%2").arg(digits[1]).arg(digits[0])).toInt();
}

/**
 * @brief CalibrationCheck::initCheck2
 * @return
 */
QGroupBox *CalibrationCheck::initCheck2()
{
    QHBoxLayout *h2Box = new QHBoxLayout();
    labCalibrKo2 = new QLabel("ЦАП-2 задание\nкалибровки");
    spinBox2 = new QSpinBox(this);
    setFormatSpinBox(spinBox2, QFont::Normal, -99999, 99999);

    buttonOk2 = new QPushButton("Чтение АЦП-2");
    buttonOk2->setAutoDefault(false);
    connect(buttonOk2, &QPushButton::pressed, this, &CalibrationCheck::setButtonReadADC2);

    labAdcCalibr2 = new QLabel("АЦП-2 калибров.");
    editAdcCalibr2 = new QLineEdit();
    setFormatLineEdit(editAdcCalibr2, "Times", 10, QFont::DemiBold, Qt::AlignCenter, 10, 140, false);
    editAdcCalibr2->setReadOnly(true);
    editAdcCalibr2->setStyleSheet("QLineEdit {background-color: #F5F6CE;}");

    labAdcDev2 = new QLabel("Погрешность, %");
    editAdcDev2 = new QLineEdit();
    editAdcDev2->setReadOnly(true);
    editAdcDev2->setStyleSheet("QLineEdit {background-color: #F79F81;}");
    //editAdcDev2->setValidator(dv);
    setFormatLineEdit(editAdcDev2, "Times", 10, QFont::QFont::Normal, Qt::AlignCenter, 10, 140, false);


    labAdcMa2 = new QLabel("Ток, ма");
    editAdcMa2 = new QLineEdit();
    //editAdcMa2->setValidator(dv);
    setFormatLineEdit(editAdcMa2, "Times", 10, QFont::DemiBold, Qt::AlignCenter, 10, 140, true);
    //editAdcMa2->setValidator(dv);

    labAdcMaDev2 = new QLabel("Погрешность, %");
    editAdcMaDev2 = new QLineEdit();
    editAdcMaDev2->setReadOnly(true);
    editAdcMaDev2->setStyleSheet("QLineEdit {background-color: #F79F81;}");
    //editAdcMaDev2->setValidator(dv);
    setFormatLineEdit(editAdcMaDev2, "Times", 10, QFont::QFont::Normal, Qt::AlignCenter, 10, 140, false);

    buttonCalc2 = new QPushButton("Расчет");
    buttonCalc2->setAutoDefault(false);
    connect(buttonCalc2, &QPushButton::pressed, this, &CalibrationCheck::getButtonCalc2);

    h2Box->addWidget(createGroupRadioBtnCheck2());
    h2Box->addWidget(makeGroupBoxCheck2(""));
    QGroupBox *grb = new QGroupBox(tr("ЦАП-2/АЦП-2"));
    grb->setLayout(h2Box);

    return grb;
}

/**
 * @brief CalibrationCheck::initBtnForm
 * @return
 */
QGroupBox *CalibrationCheck::initBtnForm(){
    QGroupBox *gBox = new QGroupBox(nullptr);
    QHBoxLayout *hBtnBox = new QHBoxLayout();
    btnCheck = new QPushButton("Контроль обмена");
    btnCheck->setAutoDefault(false);
    btnExit = new QPushButton("Выход");
    btnExit->setAutoDefault(false);
    hBtnBox->addWidget(btnCheck);
    hBtnBox->addWidget(btnExit);
    gBox->setLayout(hBtnBox);
    connect(btnCheck, &QPushButton::pressed, this, &CalibrationCheck::execCheck);
    connect(btnExit, &QPushButton::pressed, this, &CalibrationCheck::close);
    return gBox;
}


/**
 * @brief CalibrationCheck::readCalibratedADCval
 * @param sPort
 * @param modul
 * @param command
 * @return
 */
bool CalibrationCheck::readCalibratedADCval(SPort* sPort, DataModules* modul, quint8 command){
    int groupBtn = 0, numberBtn = 0, maBtn=0;
    getRadioBtnPush(idRadioBtn, &groupBtn, &numberBtn, &maBtn);

    bool retOk = false, ok;
    quint8 adrMod = static_cast<quint8>(modul->getAddrDecMod().toUInt(&ok, 10));
    const quint8 TR_SIZE = 3;
    quint8 TR[TR_SIZE]={0};

    TR[0] = adrMod;
    TR[1] = command;
    TR[2] = static_cast<quint8>(sPort->crc( reinterpret_cast<const char *>(TR), TR_SIZE));
    retOk =  sPort->writeSPort(TR, TR_SIZE);
    if(retOk) {
        retOk = sPort->getDataFromPortMBAC(adrMod, 7);
        quint16 d01LoHi = convertByteToInt16(static_cast<quint8>(sPort->RX[2]),
                                             static_cast<quint8>(sPort->RX[3]));
        double ma01 = static_cast<double>(d01LoHi) * 0.005;
        quint16 d23LoHi = convertByteToInt16(static_cast<quint8>(sPort->RX[4]),
                                             static_cast<quint8>(sPort->RX[5]));
        double ma23 = static_cast<double>(d23LoHi) * 0.005;
        if(groupBtn == GROUP_RADIO_BTN_DAC1){
            editAdcCalibr1->setText(QString().setNum(ma01, 'g', 3));
            editAdcCalibr1->setText(editAdcCalibr1->text()+setZero(editAdcCalibr1));
        }
        else if(groupBtn == GROUP_RADIO_BTN_DAC2){
            editAdcCalibr2->setText(QString().setNum(ma23, 'g', 3));
            editAdcCalibr2->setText(editAdcCalibr2->text()+setZero(editAdcCalibr2));
        }
    }
    return retOk;
}


/**
 * @brief CalibrationCheck::createGroupRadioBtnCheck2
 * @return
 */
QGroupBox *CalibrationCheck::createGroupRadioBtnCheck2()
{
    QGroupBox *groupBox = new QGroupBox(tr("Ток, мА"));
    btnGroupCheck2 = new QButtonGroup(this);

    QRadioButton *radio1_1ma  = new QRadioButton(tr("1 mA"));
    btnGroupCheck2->addButton(radio1_1ma,   2102);
    QRadioButton *radio2_4ma  = new QRadioButton(tr("4 mA"));
    btnGroupCheck2->addButton(radio2_4ma,   2204);
    QRadioButton *radio3_8ma  = new QRadioButton(tr("8 mA"));
    btnGroupCheck2->addButton(radio3_8ma,   2308);
    QRadioButton *radio4_12ma = new QRadioButton(tr("12 mA"));
    btnGroupCheck2->addButton(radio4_12ma,  2412);
    QRadioButton *radio5_16ma = new QRadioButton(tr("16 mA"));
    btnGroupCheck2->addButton(radio5_16ma,  2516);
    QRadioButton *radio6_20ma = new QRadioButton(tr("20 mA"));
    btnGroupCheck2->addButton(radio6_20ma,  2620);

    connect(btnGroupCheck2, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &CalibrationCheck::getClick);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1_1ma);
    vbox->addWidget(radio2_4ma);
    vbox->addWidget(radio3_8ma);
    vbox->addWidget(radio4_12ma);
    vbox->addWidget(radio5_16ma);
    vbox->addWidget(radio6_20ma);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

  return groupBox;
}

/**
 * @brief CalibrationCheck::makeGroupBoxCheck2
 * @param name
 * @return
 */
QGroupBox *CalibrationCheck::makeGroupBoxCheck2(QString name)
{
      QGroupBox *gBox = new QGroupBox(name);
      QFormLayout *formLay = new QFormLayout();

      formLay->addRow(labCalibrKo2);
      formLay->addRow(spinBox2, buttonOk2);
      formLay->addRow(labAdcCalibr2, labAdcDev2);
      formLay->addRow(editAdcCalibr2, editAdcDev2);
      formLay->addRow(labAdcMa2, labAdcMaDev2);
      formLay->addRow(editAdcMa2, editAdcMaDev2);
      formLay->insertRow(6, buttonCalc2);

      gBox->setLayout(formLay);
      return gBox;
}

/**
 * @brief CalibrationCheck::checkReadBetwenModuls
 * @param sPort
 * @param modul
 * @param command
 * @return
 */
quint8 CalibrationCheck::checkReadBetwenModuls(SPort* sPort, DataModules* modul, quint8 command){

    bool retOk = false, ok;
    quint8 retVal = 0;
    quint8 adrMod = static_cast<quint8>(modul->getAddrDecMod().toUInt(&ok, 10));
    const quint8 TR_SIZE = 3;
    quint8 TR[TR_SIZE]={0};
    TR[0] = adrMod;
    TR[1] = command;
    TR[2] = static_cast<quint8>(sPort->crc( reinterpret_cast<const char *>(TR), TR_SIZE));
    retOk =  sPort->writeSPort(TR, TR_SIZE);
    if(retOk) {
       retOk = sPort->getDataFromPortMBAC(adrMod, 7);
       if(retOk){
           quint8 answer = static_cast<quint8>(sPort->RX[1]);
           retVal = answer;
       }
    }
    return retVal;
}

/**
 * @brief CalibrationCheck::getMessageFromAnsver
 * @param sPort
 * @param modul
 * @param command
 * @return
 */
QString CalibrationCheck::getMessageFromAnsver(SPort* sPort, DataModules* modul){
    int numLine = modul->getNumLine().toInt();
    quint8 sign = 0;
    QString message = nullptr;

    if(numLine == 1){
        sign = checkReadBetwenModuls(sPort, modul, 0x40);
    }
    else if(numLine == 2){
        sign = checkReadBetwenModuls(sPort, modul, 0x80);
    }

    message = QStringLiteral("Линия: %1 ").arg(numLine);
    message += "Ответ: 0X" + QString::number(sign, 16) + ".";
    message += ((sign & 0x0F) == 6) ? " Нет соседнего модуля." : " Есть соседний модуль.";
    return message;
}






