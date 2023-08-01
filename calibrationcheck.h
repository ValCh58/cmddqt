#ifndef CALIBRATIONCHECK_H
#define CALIBRATIONCHECK_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QIcon>
#include <QLineEdit>
#include <QSpinBox>
#include <QGroupBox>
#include <QButtonGroup>

#include "datamodules.h"
#include "sport.h"


class CalibrationCheck : public QDialog
{
    Q_OBJECT
public:
    explicit CalibrationCheck(SPort *port, DataModules *data, QWidget *parent=nullptr, Qt::WindowFlags f=nullptr);
    ~CalibrationCheck();
    DataModules *modul;
    SPort *sport;

private:
    QButtonGroup *btnGroupCheck1;
    QButtonGroup *btnGroupCheck2;
    int idRadioBtn;//id радио кнопки//

    QLabel *labCalibrKo1;
    QSpinBox *spinBox1;
    QPushButton *buttonOk1;
    QLabel *labAdcCalibr1;
    QLineEdit *editAdcCalibr1;
    QLabel *labAdcDev1;
    QLineEdit *editAdcDev1;
    QLabel *labAdcMa1;
    QLineEdit *editAdcMa1;
    QLabel *labAdcMaDev1;
    QLineEdit *editAdcMaDev1;
    QPushButton *buttonCalc1;
    QGroupBox *makeGroupBoxCheck1(QString name);


    QLabel *labCalibrKo2;
    QSpinBox *spinBox2;
    QPushButton *buttonOk2;
    QLabel *labAdcCalibr2;
    QLineEdit *editAdcCalibr2;
    QLabel *labAdcDev2;
    QLineEdit *editAdcDev2;
    QLabel *labAdcMa2;
    QLineEdit *editAdcMa2;
    QLabel *labAdcMaDev2;
    QLineEdit *editAdcMaDev2;
    QPushButton *buttonCalc2;
    QGroupBox *makeGroupBoxCheck2(QString name);

    QPushButton *btnCheck;
    QPushButton *btnExit;

    QGroupBox *initBtnForm();
    void getRadioBtnPush(int idBtn, int *group, int *number, int *ma);
    void setFormatLineEdit(QLineEdit *qle, QString nmfont, int size, int bold, Qt::Alignment align, int len, int width, bool clbtn);
    void setFormatSpinBox(QSpinBox *sb, int bold, int min, int max);
    void showDialogInform(QMessageBox::Icon iconType, QString qstr);
    QGroupBox *initCheck1();
    QGroupBox *createGroupRadioBtnCheck1();
    QGroupBox *initCheck2();
    QGroupBox *createGroupRadioBtnCheck2();
    quint16 getGivenValue(const int selector);

    bool sendCommandDac12(SPort *sPort, DataModules *modul, quint8 command, quint16 datLoHi01, quint16 datLoHi23);
    bool readCalibratedADCval(SPort* sPort, DataModules* modul, quint8 command);
    void setButtonOk1();
    void setButtonOk2();
    void getButtonCalc(QLineEdit *editAdcMa, QLineEdit *editAdcCalibr, QLineEdit *editAdcDev, QLineEdit *editAdcMaDev, int numGroup);
    double getDeviation(double dac, double device, double ma);
    double getMa(const int selector);

    void clearText(QLineEdit *txt1, QLineEdit *txt2, QLineEdit *txt3, QLineEdit *txt4);
    quint8 checkReadBetwenModuls(SPort *sPort, DataModules *modul, quint8 command);
    QString getMessageFromAnsver(SPort* sPort, DataModules* modul);
signals:

private slots:
    void getClick(int id);
    void execCheck();
    void setButtonReadADC1();
    void setButtonReadADC2();
    void getButtonCalc1();
    void getButtonCalc2();

};

#endif // CALIBRATIONCHECK_H
