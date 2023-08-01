#include "daccalibration.h"

#include <QLabel>
#include <QPushButton>



DacCalibration::DacCalibration(QWidget *parent) : QWidget(parent)
{
   init();

}

DacCalibration::~DacCalibration(){};

QLineEdit *DacCalibration::getEditCurrValue() const
{
    return editCurrValue;
}
void DacCalibration::setEditCurrValue(QLineEdit *value)
{
    editCurrValue = value;
}

QSpinBox *DacCalibration::getSpinBox() const
{
    return spinBox;
}
void DacCalibration::setSpinBox(QSpinBox *value)
{
    spinBox = value;
}
void DacCalibration::init()
{
    QHBoxLayout *h1Box = new QHBoxLayout();
      labCurrValue = new QLabel("Измеренное значение тока:");
      editCurrValue = new QLineEdit();
      buttonOK = new QPushButton("Ok");
      labCalibrKo = new QLabel("Калибровочный коофициент:");
      spinBox = new QSpinBox(this);
      buttonWrite = new QPushButton("Записать");
    h1Box->addWidget(createGroupRadioBtn());
    h1Box->addWidget(makeGroupBox("Калибровка ЦАП1"));

    QHBoxLayout *hBox1 = new QHBoxLayout();
      hBox1->addLayout(h1Box);

    QVBoxLayout *vMainBox = new QVBoxLayout();
      vMainBox->addLayout(hBox1);
      setLayout(vMainBox);
}

QGroupBox *DacCalibration::createGroupRadioBtn()
{
    QGroupBox *groupBox = new QGroupBox(tr("Значения тока, мА"));

    QRadioButton *radio1_2ma  = new QRadioButton(tr("2 mA"));
    QRadioButton *radio2_4ma  = new QRadioButton(tr("4 mA"));
    QRadioButton *radio3_8ma  = new QRadioButton(tr("8 mA"));
    QRadioButton *radio4_12ma = new QRadioButton(tr("12 mA"));
    QRadioButton *radio5_16ma = new QRadioButton(tr("16 mA"));
    QRadioButton *radio6_20ma = new QRadioButton(tr("20 mA"));

    radio1_2ma->setChecked(true);
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

QGroupBox *DacCalibration::makeGroupBox(QString name)
{
    QGroupBox *gBox = new QGroupBox(name);
    QFormLayout *formLay = new QFormLayout();

    formLay->addRow((QWidget*)labCurrValue);
    formLay->addRow(editCurrValue, (QWidget*)buttonOK);
    formLay->addRow((QWidget*)labCalibrKo);
    formLay->addRow(spinBox, (QWidget*)buttonWrite);

    gBox->setLayout(formLay);
    return gBox;

}




