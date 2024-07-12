#include <QtMath>
#include <QButtonGroup>

#include "calibrationmbac.h"
#include "ccmbb.h"
#include "sport.h"
#include "qdebug.h"
#include "calibrationcheck.h"


/**
* @brief setZero
* @param field
* @return
*/
QString setZero(QLineEdit *field){
    int lenStr = field->text().length();
    int pos = field->text().indexOf('.')+1;
    QString numStr = (pos == 0?".000":(lenStr-pos == 0?"000":(lenStr-pos == 1?"00":(lenStr-pos == 2?"0":""))));
    return numStr;
}


/**
 * @brief CalibrationMBAC::CalibrationMBAC
 * @param data
 * @param parent
 * @param f
 */
CalibrationMBAC::CalibrationMBAC(DataModules *data, QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent, f)
{
    setWindowTitle("Настройка модуля МВАС");
    setWindowIcon(QIcon(":/cmbbImages/mbb1.png"));
    MainWindow *mainWin = static_cast<MainWindow*>(parent);
    sPorts = mainWin->ports;
    modul = data;
    countDAC1 = 0;
    countDAC2 = 0;
    countDAC3 = 0;
    countDAC4 = 0;
    prevDAC1  = 0;
    prevDAC2  = 0;
    prevDAC3  = 0;
    prevDAC4  = 0;
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(initDAC1(),0,0);
    grid->addWidget(initDAC2(),0,1);
    grid->addWidget(initADC1(),1,0);
    grid->addWidget(initADC2(),1,1);
    grid->addWidget(initBtnForm(), 2, 1);
    setLayout(grid);
    setFocusPolicy(Qt::StrongFocus);
}

/**
 * @brief CalibrationMBAC::~CalibrationMBAC
 */
CalibrationMBAC::~CalibrationMBAC(){
   // if(modul) delete modul;
    if(labCurrValue1) delete labCurrValue1;
    if(editCurrValue1) delete editCurrValue1;
    if(buttonOK1) delete buttonOK1;
    if(labCalibrKo1) delete labCalibrKo1;
    if(spinBox1) delete spinBox1;
    if(buttonWrite1) delete buttonWrite1;
    if(labHandCorr1) delete labHandCorr1;
    if(editHandCorr1) delete editHandCorr1;

    if(buttonReset1) delete buttonReset1;
    if(labCurrValue2) delete labCurrValue2;
    if(editCurrValue2) delete editCurrValue2;
    if(buttonOK2) delete buttonOK2;
    if(labCalibrKo2) delete labCalibrKo2;
    if(spinBox2) delete spinBox2;
    if(buttonWrite2) delete buttonWrite2;
    if(labHandCorr2) delete labHandCorr2;
    if(editHandCorr2) delete editHandCorr2;
    if(buttonReset2) delete buttonReset2;

    if(labCurrValue3) delete labCurrValue3;
    if(editCurrValue3) delete editCurrValue3;
    if(buttonOK3) delete buttonOK3;
    if(labAdc3) delete labAdc3;
    if(editAdc3) delete editAdc3;
    if(labAdcMa3) delete labAdcMa3;
    if(labCalibrKo3) delete labCalibrKo3;
    if(spinBox3) delete spinBox3;
    if(buttonWrite3) delete buttonWrite3;
    if(labHandCorr3) delete labHandCorr3;
    if(editHandCorr3) delete editHandCorr3;
    if(buttonReset3) delete buttonReset3;

    if(labCurrValue4) delete labCurrValue4;
    if(editCurrValue4) delete editCurrValue4;
    if(buttonOK4) delete buttonOK4;
    if(labAdc4) delete labAdc4;
    if(editAdc4) delete editAdc4;
    if(labAdcMa4) delete labAdcMa4;
    if(labCalibrKo4) delete labCalibrKo4;
    if(spinBox4) delete spinBox4;
    if(buttonWrite4) delete buttonWrite4;
    if(labHandCorr4) delete labHandCorr4;
    if(editHandCorr4) delete editHandCorr4;
    if(buttonReset4) delete buttonReset4;
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
void CalibrationMBAC::setFormatLineEdit(QLineEdit *qle, QString nmfont, int size,
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
void CalibrationMBAC::setFormatSpinBox(QSpinBox *sb, int bold, int min, int max){
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
void CalibrationMBAC::showDialogInform(QMessageBox::Icon iconType, QString qstr){
    QMessageBox qMsg;
    qMsg.setIcon(iconType);
    qMsg.setText(qstr);
    qMsg.exec();
}

/**
 * @brief CalibrationMBAC::getMessageFromOkBtn
 * @param obj
 * @return
 */
bool CalibrationMBAC::getMessageFromOkBtn(QLineEdit *obj){
    bool ok = false;
    if(obj->text().isEmpty()){
        showDialogInform(QMessageBox::Warning, "Заполните поле \"Измеренное значение тока\".");
        obj->setFocus();
        obj->setSelection(0, 7);
        ok = true;
    }
    obj->setText(obj->text().replace(',', '.'));
    return ok;
}

/**
 * @brief CalibrationMBAC::initDAC1
 * @return
 */
QGroupBox* CalibrationMBAC::initDAC1()
{
    QHBoxLayout *h1Box = new QHBoxLayout();
      labCurrValue1 = new QLabel("Измеренное значение тока:");
      editCurrValue1 = new QLineEdit();
      //QDoubleValidator *dv = new QDoubleValidator(0.0, 99.0, 3);
      //dv->setNotation(QDoubleValidator::StandardNotation);
      //editCurrValue1->setValidator(dv);
      setFormatLineEdit(editCurrValue1, "Times", 10, QFont::DemiBold, Qt::AlignCenter, 10, 140, true);
      buttonOK1 = new QPushButton("Ok");
      buttonOK1->setAutoDefault(false);
      labCalibrKo1 = new QLabel("Калибровочный коофициент:");
      spinBox1 = new QSpinBox(this);
      setFormatSpinBox(spinBox1, QFont::Normal, -99999, 99999);
      buttonWrite1 = new QPushButton("Записать");
      buttonWrite1->setAutoDefault(false);
      labHandCorr1 = new QLabel("Изменения коэф:");
      editHandCorr1 = new QLineEdit();
      setFormatLineEdit(editHandCorr1, "Times", 10, QFont::DemiBold, Qt::AlignCenter, 10, 100, false);
      editHandCorr1->setReadOnly(true);
      editHandCorr1->setStyleSheet("QLineEdit {background-color: #F79F81;}");
      buttonReset1 = new QPushButton("Сброс");
      buttonReset1->setAutoDefault(false);
    h1Box->addWidget(createGroupRadioBtn());
    h1Box->addWidget(makeGroupBox("Параметры"));
    QGroupBox *grb = new QGroupBox(tr("Калибровка ЦАП-1"));
    grb->setLayout(h1Box);
    connect(buttonOK1, &QPushButton::pressed, this, &CalibrationMBAC::setButtonDAC1OK);
    connect(buttonWrite1, &QPushButton::pressed, this, &CalibrationMBAC::setButtonDAC1Write);
    connect(spinBox1, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CalibrationMBAC::getClickSpinBoxDAC1);
    connect(buttonReset1, &QPushButton::pressed, this, &CalibrationMBAC::setButtonResetAllDAC1);

    return grb;
}

/**
 * @brief CalibrationMBAC::initDAC2
 * @return
 */
QGroupBox* CalibrationMBAC::initDAC2()
{
    QHBoxLayout *h1Box = new QHBoxLayout();
      labCurrValue2 = new QLabel("Измеренное значение тока:");
      editCurrValue2 = new QLineEdit();
      setFormatLineEdit(editCurrValue2, "Times", 10, QFont::DemiBold, Qt::AlignCenter, 10, 140, true);
      buttonOK2 = new QPushButton("Ok");
      buttonOK2->setAutoDefault(false);
      labCalibrKo2 = new QLabel("Калибровочный коофициент:");
      spinBox2 = new QSpinBox(this);
      setFormatSpinBox(spinBox2, QFont::Normal, -99999, 99999);
      buttonWrite2 = new QPushButton("Записать");
      buttonWrite2->setAutoDefault(false);
      labHandCorr2 = new QLabel("Изменения коэф:");
      editHandCorr2 = new QLineEdit();
      setFormatLineEdit(editHandCorr2, "Times", 10, QFont::DemiBold, Qt::AlignCenter, 10, 100, false);
      editHandCorr2->setReadOnly(true);
      editHandCorr2->setStyleSheet("QLineEdit {background-color: #F79F81;}");
      buttonReset2 = new QPushButton("Сброс");
      buttonReset2->setAutoDefault(false);
    h1Box->addWidget(createGroupRadioBtn2());
    h1Box->addWidget(makeGroupBox2("Параметры"));
    QGroupBox *grb = new QGroupBox(tr("Калибровка ЦАП-2"));
    grb->setLayout(h1Box);
    connect(buttonOK2, &QPushButton::pressed, this, &CalibrationMBAC::setButtonDAC2OK);
    connect(buttonWrite2, &QPushButton::pressed, this, &CalibrationMBAC::setButtonDAC2Write);
    connect(spinBox2, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CalibrationMBAC::getClickSpinBoxDAC2);
    connect(buttonReset2, &QPushButton::pressed, this, &CalibrationMBAC::setButtonResetAllDAC2);
    grb->setTabOrder(editCurrValue2, buttonOK2);
    grb->setTabOrder(buttonOK2, spinBox2);
    grb->setTabOrder(spinBox2, buttonWrite2);
    return grb;
}

/**
 * @brief CalibrationMBAC::initADC1
 * @return
 */
QGroupBox* CalibrationMBAC::initADC1()
{
    QHBoxLayout *h1Box = new QHBoxLayout();
      labCurrValue3 = new QLabel("Измеренное значение тока:");
      editCurrValue3 = new QLineEdit();
      setFormatLineEdit(editCurrValue3, "Times", 10, QFont::DemiBold, Qt::AlignCenter, 10, 140, true);
      buttonOK3 = new QPushButton("Ok");
      buttonOK3->setAutoDefault(false);

      labAdc3 = new QLabel("АЦП-1:");
      editAdc3 = new QLineEdit();
      setFormatLineEdit(editAdc3, "Times", 10, QFont::DemiBold, Qt::AlignCenter, 10, 140, true);
      editAdc3->setReadOnly(true);
      editAdc3->setStyleSheet("QLineEdit {background-color: #F5F6CE;}");

      labAdcMa3 = new QLabel("Ma");
      labCalibrKo3 = new QLabel("Калибровочный коофициент:");
      spinBox3 = new QSpinBox(this);
      setFormatSpinBox(spinBox3, QFont::Normal, -99999, 99999);
      buttonWrite3 = new QPushButton("Записать");
      buttonWrite3->setAutoDefault(false);

      labHandCorr3 = new QLabel("Изменения коэф:");
      editHandCorr3 = new QLineEdit();
      setFormatLineEdit(editHandCorr3, "Times", 10, QFont::DemiBold, Qt::AlignCenter, 10, 100, false);
      editHandCorr3->setReadOnly(true);
      editHandCorr3->setStyleSheet("QLineEdit {background-color: #F79F81;}");
      buttonReset3 = new QPushButton("Сброс");
      buttonReset3->setAutoDefault(false);

      h1Box->addWidget(createGroupRadioBtn1_1());
    h1Box->addWidget(makeGroupBox1_1("Параметры"));
    QGroupBox *grb = new QGroupBox(tr("Калибровка АЦП-1"));
    grb->setLayout(h1Box);
    connect(buttonOK3, &QPushButton::pressed, this, &CalibrationMBAC::setButtonADC1OK);
    connect(buttonWrite3, &QPushButton::pressed, this, &CalibrationMBAC::setButtonADC1Write);
    connect(spinBox3, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CalibrationMBAC::getClickSpinBoxADC1);
    connect(buttonReset3, &QPushButton::pressed, this, &CalibrationMBAC::setButtonResetAllADC1);
    return grb;
}

/**
 * @brief CalibrationMBAC::initADC2
 * @return
 */
QGroupBox* CalibrationMBAC::initADC2()
{
    QHBoxLayout *h1Box = new QHBoxLayout();
      labCurrValue4 = new QLabel("Измеренное значение тока:");
      editCurrValue4 = new QLineEdit();
      setFormatLineEdit(editCurrValue4, "Times", 10, QFont::DemiBold, Qt::AlignCenter, 10, 140, true);
      buttonOK4 = new QPushButton("Ok");
      buttonOK4->setAutoDefault(false);

      labAdc4 = new QLabel("АЦП-2:");
      editAdc4 = new QLineEdit();
      setFormatLineEdit(editAdc4, "Times", 10, QFont::DemiBold, Qt::AlignCenter, 10, 140, true);
      editAdc4->setReadOnly(true);
      editAdc4->setStyleSheet("QLineEdit {background-color: #F5F6CE;}");

      labAdcMa4 = new QLabel("Ma");
      labCalibrKo4 = new QLabel("Калибровочный коофициент:");
      spinBox4 = new QSpinBox(this);
      setFormatSpinBox(spinBox4, QFont::Normal, -99999, 99999);

      buttonWrite4 = new QPushButton("Записать");
      buttonWrite4->setAutoDefault(false);
      labHandCorr4 = new QLabel("Изменения коэф:");
      editHandCorr4 = new QLineEdit();
      setFormatLineEdit(editHandCorr4, "Times", 10, QFont::DemiBold, Qt::AlignCenter, 10, 100, false);
      editHandCorr4->setReadOnly(true);
      editHandCorr4->setStyleSheet("QLineEdit {background-color: #F79F81;}");
      buttonReset4 = new QPushButton("Сброс");
      buttonReset4->setAutoDefault(false);
    h1Box->addWidget(createGroupRadioBtn1_2());
    h1Box->addWidget(makeGroupBox1_2("Параметры"));
    QGroupBox *grb = new QGroupBox(tr("Калибровка АЦП-2"));
    grb->setLayout(h1Box);
    connect(buttonOK4, &QPushButton::pressed, this, &CalibrationMBAC::setButtonADC2OK);
    connect(buttonWrite4, &QPushButton::pressed, this, &CalibrationMBAC::setButtonADC2Write);
    connect(spinBox4, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CalibrationMBAC::getClickSpinBoxADC2);
    connect(buttonReset4, &QPushButton::pressed, this, &CalibrationMBAC::setButtonResetAllADC2);
    return grb;
}

/**
 * @brief CalibrationMBAC::initBtnForm
 * @return
 */
QGroupBox *CalibrationMBAC::initBtnForm(){
    QGroupBox *gBox = new QGroupBox(nullptr);
    QHBoxLayout *hBtnBox = new QHBoxLayout();
    btnExecCheckDialog = new QPushButton("Контроль");
    btnExecCheckDialog->setAutoDefault(false);
    btnExit = new QPushButton("Выход");
    btnExit->setAutoDefault(false);
    hBtnBox->addWidget(btnExecCheckDialog);
    hBtnBox->addWidget(btnExit);
    gBox->setLayout(hBtnBox);
    connect(btnExecCheckDialog, &QPushButton::pressed, this, &CalibrationMBAC::execFormCheck);
    connect(btnExit, &QPushButton::pressed, this, &CalibrationMBAC::close);
    return gBox;
}

/**
 * @brief CalibrationMBAC::execFormCheck
 */
void CalibrationMBAC::execFormCheck(){
    if(modul->isData()){
       int numCom = modul->getNumLine().toInt()-1;
       calibrCheck = new CalibrationCheck(sPorts.at(numCom), modul, this);
       calibrCheck->exec();
       calibrCheck->setAttribute(Qt::WA_DeleteOnClose);
    }
}

/**
 * @brief CalibrationMBAC::createGroupRadioBtn
 * @return
 */
QGroupBox *CalibrationMBAC::createGroupRadioBtn()
{
      QGroupBox *groupBox = new QGroupBox(tr("Ток, мА"));
      btnGroupDAC1 = new QButtonGroup(this);

      QRadioButton *radio1_2ma  = new QRadioButton(tr("1 mA"));
      btnGroupDAC1->addButton(radio1_2ma, 1102);
      QRadioButton *radio2_4ma  = new QRadioButton(tr("4 mA"));
       btnGroupDAC1->addButton(radio2_4ma, 1204);
      QRadioButton *radio3_8ma  = new QRadioButton(tr("8 mA"));
       btnGroupDAC1->addButton(radio3_8ma, 1308);
      QRadioButton *radio4_12ma = new QRadioButton(tr("12 mA"));
       btnGroupDAC1->addButton(radio4_12ma, 1412);
      QRadioButton *radio5_16ma = new QRadioButton(tr("16 mA"));
       btnGroupDAC1->addButton(radio5_16ma, 1516);
      QRadioButton *radio6_20ma = new QRadioButton(tr("20 mA"));
       btnGroupDAC1->addButton(radio6_20ma, 1620);

     connect(btnGroupDAC1, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
              this, &CalibrationMBAC::getClick);

      QVBoxLayout *vbox = new QVBoxLayout;
      vbox->addWidget(radio1_2ma);
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
 * @brief CalibrationMBAC::createGroupRadioBtn2
 * @return
 */
QGroupBox *CalibrationMBAC::createGroupRadioBtn2()
{
      QGroupBox *groupBox = new QGroupBox(tr("Ток, мА"));
      btnGroupDAC2 = new QButtonGroup(this);

      QRadioButton *radio1_2ma  = new QRadioButton(tr("1 mA"));
      btnGroupDAC2->addButton(radio1_2ma,  2102);
      QRadioButton *radio2_4ma  = new QRadioButton(tr("4 mA"));
      btnGroupDAC2->addButton(radio2_4ma,  2204);
      QRadioButton *radio3_8ma  = new QRadioButton(tr("8 mA"));
      btnGroupDAC2->addButton(radio3_8ma,  2308);
      QRadioButton *radio4_12ma = new QRadioButton(tr("12 mA"));
      btnGroupDAC2->addButton(radio4_12ma, 2412);
      QRadioButton *radio5_16ma = new QRadioButton(tr("16 mA"));
      btnGroupDAC2->addButton(radio5_16ma, 2516);
      QRadioButton *radio6_20ma = new QRadioButton(tr("20 mA"));
      btnGroupDAC2->addButton(radio6_20ma, 2620);

      connect(btnGroupDAC2, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
              this, &CalibrationMBAC::getClick);

      QVBoxLayout *vbox = new QVBoxLayout;
      vbox->addWidget(radio1_2ma);
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
 * @brief CalibrationMBAC::createGroupRadioBtn1_1
 * @return
 */
QGroupBox *CalibrationMBAC::createGroupRadioBtn1_1()
{
      QGroupBox *groupBox = new QGroupBox(tr("Ток, мА"));
      btnGroupADC1 = new QButtonGroup(this);

      QRadioButton *radio1_2ma  = new QRadioButton(tr("1 mA"));
      btnGroupADC1->addButton(radio1_2ma,  3102);
      QRadioButton *radio2_4ma  = new QRadioButton(tr("4 mA"));
      btnGroupADC1->addButton(radio2_4ma,  3204);
      QRadioButton *radio3_8ma  = new QRadioButton(tr("8 mA"));
      btnGroupADC1->addButton(radio3_8ma,  3308);
      QRadioButton *radio4_12ma = new QRadioButton(tr("12 mA"));
      btnGroupADC1->addButton(radio4_12ma,  3412);
      QRadioButton *radio5_16ma = new QRadioButton(tr("16 mA"));
      btnGroupADC1->addButton(radio5_16ma,  3516);
      QRadioButton *radio6_20ma = new QRadioButton(tr("20 mA"));
      btnGroupADC1->addButton(radio6_20ma,  3620);

      connect(btnGroupADC1, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
              this, &CalibrationMBAC::getClick);

      QVBoxLayout *vbox = new QVBoxLayout;
      vbox->addWidget(radio1_2ma);
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
 * @brief CalibrationMBAC::createGroupRadioBtn1_2
 * @return
 */
QGroupBox *CalibrationMBAC::createGroupRadioBtn1_2()
{
      QGroupBox *groupBox = new QGroupBox(tr("Ток, мА"));
      btnGroupADC2 = new QButtonGroup(this);

      QRadioButton *radio1_2ma  = new QRadioButton(tr("1 mA"));
      btnGroupADC2->addButton(radio1_2ma,  4102);
      QRadioButton *radio2_4ma  = new QRadioButton(tr("4 mA"));
      btnGroupADC2->addButton(radio2_4ma,  4204);
      QRadioButton *radio3_8ma  = new QRadioButton(tr("8 mA"));
      btnGroupADC2->addButton(radio3_8ma,  4308);
      QRadioButton *radio4_12ma = new QRadioButton(tr("12 mA"));
      btnGroupADC2->addButton(radio4_12ma,  4412);
      QRadioButton *radio5_16ma = new QRadioButton(tr("16 mA"));
      btnGroupADC2->addButton(radio5_16ma,  4516);
      QRadioButton *radio6_20ma = new QRadioButton(tr("20 mA"));
      btnGroupADC2->addButton(radio6_20ma,  4620);

     connect(btnGroupADC2, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
              this, &CalibrationMBAC::getClick);

      QVBoxLayout *vbox = new QVBoxLayout;
      vbox->addWidget(radio1_2ma);
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
 * @brief CalibrationMBAC::makeGroupBox
 * @param name
 * @return
 */
QGroupBox *CalibrationMBAC::makeGroupBox(QString name)
{
      QGroupBox *gBox = new QGroupBox(name);
      QFormLayout *formLay = new QFormLayout();

      formLay->addRow(labCurrValue1);
      formLay->addRow(editCurrValue1, buttonOK1);
      formLay->addRow(labCalibrKo1);
      formLay->addRow(spinBox1, buttonWrite1);
      formLay->addRow(labHandCorr1);
      formLay->addRow(editHandCorr1, buttonReset1);
      gBox->setLayout(formLay);
      return gBox;
}

/**
 * @brief CalibrationMBAC::makeGroupBox2
 * @param name
 * @return
 */
QGroupBox *CalibrationMBAC::makeGroupBox2(QString name)
{
      QGroupBox *gBox = new QGroupBox(name);
      QFormLayout *formLay = new QFormLayout();

      formLay->addRow(labCurrValue2);
      formLay->addRow(editCurrValue2, buttonOK2);
      formLay->addRow(labCalibrKo2);
      formLay->addRow(spinBox2, buttonWrite2);
      formLay->addRow(labHandCorr2);
      formLay->addRow(editHandCorr2, buttonReset2);
      gBox->setLayout(formLay);
      return gBox;
}

/**
 * @brief CalibrationMBAC::makeGroupBox1_1
 * @param name
 * @return
 */
QGroupBox *CalibrationMBAC::makeGroupBox1_1(QString name)
{
      QGroupBox *gBox = new QGroupBox(name);
      QFormLayout *formLay = new QFormLayout();

      formLay->addRow(labCurrValue3);
      formLay->addRow(editCurrValue3, buttonOK3);
      formLay->addRow(labAdc3);
      formLay->addRow(editAdc3, labAdcMa3);
      formLay->addRow(labCalibrKo3);
      formLay->addRow(spinBox3, buttonWrite3);
      formLay->addRow(labHandCorr3);
      formLay->addRow(editHandCorr3, buttonReset3);
      gBox->setLayout(formLay);
      return gBox;
}

/**
 * @brief CalibrationMBAC::makeGroupBox1_2
 * @param name
 * @return
 */
QGroupBox *CalibrationMBAC::makeGroupBox1_2(QString name)
{
      QGroupBox *gBox = new QGroupBox(name);
      QFormLayout *formLay = new QFormLayout();

      formLay->addRow(labCurrValue4);
      formLay->addRow(editCurrValue4, buttonOK4);
      formLay->addRow(labAdc4);
      formLay->addRow(editAdc4, labAdcMa4);
      formLay->addRow(labCalibrKo4);
      formLay->addRow(spinBox4, buttonWrite4);
      formLay->addRow(labHandCorr4);
      formLay->addRow(editHandCorr4, buttonReset4);
      gBox->setLayout(formLay);
      return gBox;
}

/**
 * Slot for buttonOK1 *
 * @brief CalibrationMBAC::setButtonDAC1OK
 */
void CalibrationMBAC::setButtonDAC1OK(){
    if(getMessageFromOkBtn(editCurrValue1)){return;}
    int groupBtn = 0, numberBtn = 0, maBtn=0;
    getRadioBtnPush(idRadioBtn, &groupBtn, &numberBtn, &maBtn);
    double iGiven = getGivenValue(maBtn);
    quint16 resKuk = 0;
    double measured = editCurrValue1->text().toDouble();
    if(groupBtn == GROUP_RADIO_BTN_DAC1){
       resKuk = calcKUKDac(iGiven, measured);
       spinBox1->setValue(resKuk);
       buttonWrite1->setDefault(true);
    }
    else{
         showDialogInform(QMessageBox::Warning, "Выбрано значение тока для ЦАП2.");
    }
    setButtonResetDAC1();
}

/**
 * Slot for buttonOK2 *
 * @brief CalibrationMBAC::setButtonDAC2OK
 */
void CalibrationMBAC::setButtonDAC2OK(){
    if(getMessageFromOkBtn(editCurrValue2)){return;}
    int groupBtn = 0, numberBtn = 0, maBtn=0;
    getRadioBtnPush(idRadioBtn, &groupBtn, &numberBtn, &maBtn);
    double iGiven = getGivenValue(maBtn);
    quint16 resKuk = 0;
    double measured = editCurrValue2->text().toDouble();
    if(groupBtn == GROUP_RADIO_BTN_DAC2){
       resKuk = calcKUKDac(iGiven, measured);
       spinBox2->setValue(resKuk);
       buttonWrite2->setDefault(true);
    }
    else{
         showDialogInform(QMessageBox::Warning, "Выбрано значение тока для ЦАП1.");
    }
    setButtonResetDAC2();
}

/**
 * Slot for buttonOK3 *
 * @brief CalibrationMBAC::setButtonADC1OK
 */
void CalibrationMBAC::setButtonADC1OK(){
   if(getMessageFromOkBtn(editCurrValue3)){return;}
   int groupBtn = 0, numberBtn = 0, maBtn=0;
   getRadioBtnPush(idRadioBtn, &groupBtn, &numberBtn, &maBtn);
   int numCom = modul->getNumLine().toInt()-1;
   if(groupBtn == GROUP_RADIO_BTN_ADC1){
    readUncalibratedADCval(sPorts.at(numCom), modul, 0x3E, editCurrValue3, spinBox3);
    buttonWrite3->setDefault(true);
   }else{
       showDialogInform(QMessageBox::Warning, "Выбрано значение тока для АЦП2.");
   }
   setButtonResetADC1();
}

/**
 * Slot for buttonOK4 *
 * @brief CalibrationMBAC::setButtonADC2OK
 */
void CalibrationMBAC::setButtonADC2OK(){
    if(getMessageFromOkBtn(editCurrValue4)){return;}
    int groupBtn = 0, numberBtn = 0, maBtn=0;
    getRadioBtnPush(idRadioBtn, &groupBtn, &numberBtn, &maBtn);
    int numCom = modul->getNumLine().toInt()-1;
    if(groupBtn == GROUP_RADIO_BTN_ADC2){
    readUncalibratedADCval(sPorts.at(numCom), modul, 0x3E, editCurrValue4, spinBox4);
    buttonWrite4->setDefault(true);
    }else{
        showDialogInform(QMessageBox::Warning, "Выбрано значение тока для АЦП1.");
    }
     setButtonResetADC2();
}

/**
 * Slot for buttonWrite1 *
 * @brief CalibrationMBAC::setButtonDAC1Write
 */
void CalibrationMBAC::setButtonDAC1Write(){
    bool Ok = false;
    int groupBtn = 0, numberBtn = 0, maBtn=0;
    getRadioBtnPush(idRadioBtn, &groupBtn, &numberBtn, &maBtn);
    quint8 kuk = getKUK(maBtn);
    int numCom = modul->getNumLine().toInt()-1;
    if(groupBtn == GROUP_RADIO_BTN_DAC1){
       quint16 dataKuk = static_cast<quint16>(spinBox1->value());
       Ok = sendCommandDac(sPorts.at(numCom), modul, 3, kuk, dataKuk);//getGivenValue(dataKuk));
       if(Ok){
           //showDialogInform(QMessageBox::Information, "Команда \"Запись KUK\" в ЦАП1 выполнена.");
           btnGroupDAC1->button(idRadioBtn)->setIcon(QIcon(":/cmbbImages/accept.png"));
       }
       else if(!Ok){showDialogInform(QMessageBox::Warning, "Команда \"Запись KUK\" в ЦАП1 не выполнена.");}
    }
    else{showDialogInform(QMessageBox::Warning, "Выбрано значение тока для ЦАП2.");}
}

/**
 * Slot for buttonWrite2 *
 * @brief CalibrationMBAC::setButtonDAC2Write
 */
void CalibrationMBAC::setButtonDAC2Write(){
    bool Ok = false;
    int groupBtn = 0, numberBtn = 0, maBtn=0;
    getRadioBtnPush(idRadioBtn, &groupBtn, &numberBtn, &maBtn);
    quint8 kuk = getKUK(maBtn);
    int numCom = modul->getNumLine().toInt()-1;
    if(groupBtn == GROUP_RADIO_BTN_DAC2){
       quint16 dataKuk = static_cast<quint16>(spinBox2->value());
       Ok = sendCommandDac(sPorts.at(numCom), modul, 2, kuk, dataKuk);//getGivenValue(dataKuk));
       if(Ok){
           //showDialogInform(QMessageBox::Information, "Команда \"Запись KUK\" в ЦАП2 выполнена.");
           btnGroupDAC2->button(idRadioBtn)->setIcon(QIcon(":/cmbbImages/accept.png"));
       }
       else if(!Ok){showDialogInform(QMessageBox::Warning, "Команда \"Запись KUK\" в ЦАП2 не выполнена.");}
    }
    else{
         showDialogInform(QMessageBox::Warning, "Выбрано значение тока для ЦАП1.");
    }
}



/**
 * Write KUK for ADC1 *
 * Slot for buttonWrite3 *
 * @brief CalibrationMBAC::setButtonADC1Write
 */
void CalibrationMBAC::setButtonADC1Write(){
    bool Ok = false;
    int groupBtn = 0, numberBtn = 0, maBtn=0;
    getRadioBtnPush(idRadioBtn, &groupBtn, &numberBtn, &maBtn);
    quint8 kuk = getKUK(maBtn);
    int numCom = modul->getNumLine().toInt()-1;
    if(groupBtn == GROUP_RADIO_BTN_ADC1){
       quint16 dataKuk = static_cast<quint16>(spinBox3->value());
       Ok = sendCommandDac(sPorts.at(numCom), modul, 1, kuk, dataKuk);
       if(Ok){
           //showDialogInform(QMessageBox::Information, "Команда \"Запись KUK\" в АЦП1 выполнена.");
           btnGroupADC1->button(idRadioBtn)->setIcon(QIcon(":/cmbbImages/accept.png"));
       }
       else if(!Ok){showDialogInform(QMessageBox::Warning, "Команда \"Запись KUK\" в АЦП1 не выполнена.");}
    }
    else{
         showDialogInform(QMessageBox::Warning, "Выбрано значение тока для ЦАП2.");
    }
}

/**
 * Write KUK for ADC2 *
 * Slot for buttonWrite4 *
 * @brief CalibrationMBAC::setButtonADC2Write
 */
void CalibrationMBAC::setButtonADC2Write(){
    bool Ok = false;
    int groupBtn = 0, numberBtn = 0, maBtn=0;
    getRadioBtnPush(idRadioBtn, &groupBtn, &numberBtn, &maBtn);
    quint8 kuk = getKUK(maBtn);
    int numCom = modul->getNumLine().toInt()-1;
    if(groupBtn == GROUP_RADIO_BTN_ADC2){
       quint16 dataKuk = static_cast<quint16>(spinBox4->value());
       Ok = sendCommandDac(sPorts.at(numCom), modul, 0, kuk, dataKuk);
       if(Ok){
           //showDialogInform(QMessageBox::Information, "Команда \"Запись KUK\" в АЦП2 выполнена.");
           btnGroupADC2->button(idRadioBtn)->setIcon(QIcon(":/cmbbImages/accept.png"));
       }
       else if(!Ok){showDialogInform(QMessageBox::Warning, "Команда \"Запись KUK\" в АЦП2 не выполнена.");}
    }
    else{
         showDialogInform(QMessageBox::Warning, "Выбрано значение тока для ЦАП1.");
    }
}

/**
 * @brief CalibrationMBAC::getClickSpinBoxDAC1
 * @param id
 */
void CalibrationMBAC::getClickSpinBoxDAC1(int id){
    if(id > prevDAC1)
       countDAC1++;
    else if(id < prevDAC1)
        countDAC1--;
    prevDAC1 = spinBox1->value();
    editHandCorr1->setText(QStringLiteral("%1").arg(countDAC1));
}

/**
 * @brief CalibrationMBAC::getClickSpinBoxDAC2
 * @param id
 */
void CalibrationMBAC::getClickSpinBoxDAC2(int id){
    if(id > prevDAC2)
       countDAC2++;
    else if(id < prevDAC2)
        countDAC2--;
    prevDAC2 = spinBox2->value();
    editHandCorr2->setText(QStringLiteral("%1").arg(countDAC2));
}

/**
 * @brief CalibrationMBAC::getClickSpinBoxADC1
 * @param id
 */
void CalibrationMBAC::getClickSpinBoxADC1(int id){
    if(id > prevDAC3)
       countDAC3++;
    else if(id < prevDAC3)
        countDAC3--;
    prevDAC3 = spinBox3->value();
    editHandCorr3->setText(QStringLiteral("%1").arg(countDAC3));
}

/**
 * @brief CalibrationMBAC::getClickSpinBoxADC2
 * @param id
 */
void CalibrationMBAC::getClickSpinBoxADC2(int id){
    if(id > prevDAC4)
       countDAC4++;
    else if(id < prevDAC4)
        countDAC4--;
    prevDAC4 = spinBox4->value();
    editHandCorr4->setText(QStringLiteral("%1").arg(countDAC4));
}

/**
 * @brief CalibrationMBAC::setButtonResetDAC1
 */
void CalibrationMBAC::setButtonResetDAC1(){//Обнуление поля коррекции к-та//
    countDAC1 = 0;
    prevDAC1 = spinBox1->value();
    editHandCorr1->setText(QStringLiteral("%1").arg(countDAC1));
}

/**
 * @brief CalibrationMBAC::setButtonResetAllDAC1
 */
void CalibrationMBAC::setButtonResetAllDAC1(){//Обнуление поля коррекции к-та возврат значения счетчика//
    spinBox1->setValue(prevDAC1 - countDAC1);
    countDAC1 = 0;
    prevDAC1 = spinBox1->value();
    editHandCorr1->setText(QStringLiteral("%1").arg(countDAC1));
}

/**
 * @brief CalibrationMBAC::setButtonResetDAC2
 */
void CalibrationMBAC::setButtonResetDAC2(){
    countDAC2 = 0;
    prevDAC2 = spinBox2->value();
    editHandCorr2->setText(QStringLiteral("%1").arg(countDAC2));
}

/**
 * @brief CalibrationMBAC::setButtonResetAllDAC2
 */
void CalibrationMBAC::setButtonResetAllDAC2(){
    spinBox2->setValue(prevDAC2 - countDAC2);
    countDAC2 = 0;
    prevDAC2 = spinBox2->value();
    editHandCorr2->setText(QStringLiteral("%1").arg(countDAC2));
}

/**
 * @brief CalibrationMBAC::setButtonResetADC1
 */
void CalibrationMBAC::setButtonResetADC1(){
    countDAC3 = 0;
    prevDAC3 = spinBox3->value();
    editHandCorr3->setText(QStringLiteral("%1").arg(countDAC3));
}

/**
 * @brief CalibrationMBAC::setButtonResetAllADC1
 */
void CalibrationMBAC::setButtonResetAllADC1(){
    spinBox3->setValue(prevDAC3 - countDAC3);
    countDAC3 = 0;
    prevDAC3 = spinBox3->value();
    editHandCorr3->setText(QStringLiteral("%1").arg(countDAC3));
}

/**
 * @brief CalibrationMBAC::setButtonResetADC2
 */
void CalibrationMBAC::setButtonResetADC2(){
    countDAC4 = 0;
    prevDAC4 = spinBox4->value();
    editHandCorr4->setText(QStringLiteral("%1").arg(countDAC4));
}

void CalibrationMBAC::setButtonResetAllADC2(){
    spinBox4->setValue(prevDAC4 - countDAC4);
    countDAC4 = 0;
    prevDAC4 = spinBox4->value();
    editHandCorr4->setText(QStringLiteral("%1").arg(countDAC4));
}

/**
 * Разбор Id нажатой радиокнопки *
 * @brief CalibrationMBAC::getRadioBtnPush
 * @param idBtn
 * @param group
 * @param number
 * @param ma
 */
void CalibrationMBAC::getRadioBtnPush(int idBtn, int *group, int *number, int *ma)
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
 * @brief CalibrationMBAC::getGivenValue
 * @param selector
 * @return
 */
   quint16 CalibrationMBAC::getGivenValue(const int selector){
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
 * @brief CalibrationMBAC::getKUK
 * @param selector
 * @return
 */
quint8 CalibrationMBAC::getKUK(const int selector){
    quint8 retVal = 0;
    switch(selector){
     case 2://1Ma !
         retVal = 0;
         break;
     case 4://Ma
         retVal = 1;
         break;
     case 8://Ma
         retVal = 2;
         break;
     case 12://Ma
         retVal = 3;
         break;
     case 16://Ma
         retVal = 4;
         break;
     case 20://Ma
         retVal = 5;
         break;
     }
     return retVal;
}

/**
 * @brief CalibrationMBAC::fieldСlearingDac
 * @param currVal
 * @param kukKo
 * @param changeKo
 */
void CalibrationMBAC::fieldClearingDac(QLineEdit *currVal, QSpinBox *kukKo, QLineEdit *changeKo, QLineEdit *maVal){
    currVal->clear(); currVal->setFocus();
    kukKo->clear();
    changeKo->clear();
    if(maVal != nullptr){maVal->clear();}
}

/**
 * Обработчик Слота buttonRadio для getClick btnGroupDAC1 и btnGroupDAC2, btnGroupADC1 и btnGroupADC2 *
 * @brief CalibrationMBAC::getClick
 * @param id
 */
void CalibrationMBAC::getClick(int id){
    idRadioBtn = id;//Присвоим индекс радиокнопки//
    int groupBtn = 0, numberBtn = 0, maBtn=0;
    //idRadioBtn индекс нажатой кнопки, groupBtn номер группы кнопок,
    //numberBtn номер кнопки в группе, maBtn выбранный ток калибровки//
    getRadioBtnPush(idRadioBtn, &groupBtn, &numberBtn, &maBtn);//Обработка индекса кнопки//
    int numCom = modul->getNumLine().toInt()-1;//Получим номер порта для связи//
    bool Ok = false;
    QString str = nullptr;
    quint8 numDAC = 0, specPar = 6, commandAdc = 0;
    if(groupBtn == GROUP_RADIO_BTN_DAC1 || groupBtn == GROUP_RADIO_BTN_DAC2){
       switch(groupBtn){
       case GROUP_RADIO_BTN_DAC1:
           if(btnGroupDAC1->button(id)->isChecked()){
               btnGroupDAC1->button(id)->setIcon(QIcon(""));
           }
                numDAC = 4;
                str = "ЦАП1\"";
                fieldClearingDac(editCurrValue1, spinBox1, editHandCorr1, nullptr);
                buttonOK1->setDefault(true);
           break;
       case GROUP_RADIO_BTN_DAC2:
           if(btnGroupDAC2->button(id)->isChecked()){
               btnGroupDAC2->button(id)->setIcon(QIcon(""));
           }
                numDAC = 5;
                str = "ЦАП2\"";
                fieldClearingDac(editCurrValue2, spinBox2, editHandCorr2, nullptr);
                 buttonOK2->setDefault(true);
           break;

       default: showDialogInform(QMessageBox::Information, "Команда не найдена."); return;
    }
       Ok = sendCommandDac(sPorts.at(numCom), modul, numDAC, specPar, getGivenValue(maBtn));
       if(Ok){
           ;//showDialogInform(QMessageBox::Information, "Команда \"Запись не калиброванного значения на " + str + " выполнена.");
       }
       else if(!Ok){showDialogInform(QMessageBox::Warning, "Команда \"Запись не калиброванного значения на " + str + " не выполнена.");}
       return;
    }
    if(groupBtn == GROUP_RADIO_BTN_ADC1 || groupBtn == GROUP_RADIO_BTN_ADC2){
        switch(groupBtn){
        case GROUP_RADIO_BTN_ADC1:
            if(btnGroupADC1->button(id)->isChecked()){
                btnGroupADC1->button(id)->setIcon(QIcon(""));
            }
                    fieldClearingDac(editCurrValue3, spinBox3, editHandCorr3, editAdc3);
                    buttonOK3->setDefault(true);

            break;
        case GROUP_RADIO_BTN_ADC2:
            if(btnGroupADC2->button(id)->isChecked()){
                btnGroupADC2->button(id)->setIcon(QIcon(""));
            }
                    fieldClearingDac(editCurrValue4, spinBox4, editHandCorr4, editAdc4);
                    buttonOK4->setDefault(true);
            break;
        default: showDialogInform(QMessageBox::Information, "Радиокнопка  не найдена."); return;
        }
         commandAdc = (numCom + 1) == 1 ? 0x41 : 0x81;
         Ok = sendCommandDac12(sPorts.at(numCom), modul, commandAdc, getGivenValue(maBtn), getGivenValue(maBtn));
         if(Ok){
             //showDialogInform(QMessageBox::Information, "Команда \"Запись калиброванного значения на ЦАПы\" выполнена.")
                     ;}
         else if(!Ok){showDialogInform(QMessageBox::Warning, "Команда \"Запись калиброванного значения на ЦАПы\" не выполнена.");}
    }
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
bool CalibrationMBAC::sendCommandDac12(SPort *sPort,
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
 * Запись не калиброванных значений в ЦАПы *
 * @brief CalibrationMBAC::sendCommandDac
 * @param thisPort
 * @param rowInfo
 * @param numDac
 * @param specPar
 * @param datLoHi
 * @return
 */
bool CalibrationMBAC::sendCommandDac(SPort* sPort,
                                     DataModules* modul,
                                     quint8 numDac,
                                     quint8 specPar,
                                     quint16 datLoHi)
{
   bool retOk = false, ok;
   quint8 lo = (datLoHi & 0x00FF);
   quint8 hi = (datLoHi & 0xFF00)>>8;
   quint8 adrMod = static_cast<quint8>(modul->getAddrDecMod().toUInt(&ok, 10));
   retOk = sPort->writeDataAcpMBAC(numDac, specPar, adrMod, 0x5E, lo, hi);
   if(retOk) {retOk = sPort->getDataFromPortMBAC(adrMod, 3);}
   return retOk;
}

/**
 * Чтение не калиброванных значений из АЦП *
 * @brief CalibrationMBAC::readUncalibratedADCval
 * @param sPort
 * @param modul
 * @param command
 * @return
 */
bool CalibrationMBAC::readUncalibratedADCval(SPort* sPort, DataModules* modul, quint8 command, QLineEdit *qline, QSpinBox *qspin){
    int groupBtn = 0, numberBtn = 0, maBtn=0;
    getRadioBtnPush(idRadioBtn, &groupBtn, &numberBtn, &maBtn);
    //double iGiven = getGivenValue(maBtn);! не удаляем !

    bool retOk = false, ok;
    quint8 adrMod = static_cast<quint8>(modul->getAddrDecMod().toUInt(&ok, 10));
    const quint8 TR_SIZE = 3;
    quint8 TR[TR_SIZE]={0};
    quint16 d01LoHi = 0;
    quint16 d23LoHi = 0;
    TR[0] = adrMod;
    TR[1] = command;
    TR[2] = static_cast<quint8>(sPort->crc( reinterpret_cast<const char *>(TR), TR_SIZE));
    retOk =  sPort->writeSPort(TR, TR_SIZE);
    if(retOk) {
        retOk = sPort->getDataFromPortMBAC(adrMod, 7);
        if(groupBtn == GROUP_RADIO_BTN_ADC1){
           d01LoHi = convertByteToInt16(static_cast<quint8>(sPort->RX[2]),
                                             static_cast<quint8>(sPort->RX[3]));
        double ma01 = static_cast<double>(d01LoHi) * 0.005;
        editAdc3->setText(QString().setNum(ma01, 'g', 3));
        editAdc3->setText(editAdc3->text()+setZero(editAdc3));
        }
        if(groupBtn == GROUP_RADIO_BTN_ADC2){
           d23LoHi = convertByteToInt16(static_cast<quint8>(sPort->RX[4]),
                                             static_cast<quint8>(sPort->RX[5]));
        double ma23 = static_cast<double>(d23LoHi) * 0.005;
        editAdc4->setText(QString().setNum(ma23, 'g', 3));
        editAdc4->setText(editAdc4->text()+setZero(editAdc4));

        }

        if(retOk){
            //showDialogInform(QMessageBox::Information, "Команда \"Чтение не калиброванного значения на ЦАПы\" выполнена.")
                    ;}
        else if(!retOk){showDialogInform(QMessageBox::Warning, "Команда \"Запись не калиброванного значения на ЦАПы\" не выполнена.");}
        quint16 kukADC1 = 0;
        double measured = qline->text().toDouble();
        kukADC1 = calcKUKAdc(/*iGiven,*/ measured, (groupBtn == GROUP_RADIO_BTN_ADC1 ? d01LoHi : d23LoHi));
        qspin->setValue(kukADC1);
    }
    return retOk;
}

/**
 * Расчет KUK для ЦАПов *
 * @brief CalibrationMBAC::calcKUK
 * @param idBtn
 */
quint16 CalibrationMBAC::calcKUKDac(double iGiven, double measured){
    quint16 retKuk = static_cast<quint16>(qRound(iGiven + (iGiven - (measured/0.005))));
    return retKuk;
}

/**
 * Расчет KUK для АЦП *
 * @brief CalibrationMBAC::calcKUKcalcKUKAdc
 * @param idBtn
 */
quint16 CalibrationMBAC::calcKUKAdc(/*double iGiven,*/ double measured, double adc){
    quint16 retKuk = static_cast<quint16>(qRound((measured/0.005) - (measured/0.005 - adc)));
    return retKuk;
}











