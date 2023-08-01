

#ifndef CALIBRATIONMBAC_H
#define CALIBRATIONMBAC_H

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

#include "datamodules.h"
#include "sport.h"
#include "myprogressbar.h"
#include "consts.h"
#include "calibrationcheck.h"


class CalibrationMBAC : public QDialog
{
    Q_OBJECT

public:
    CalibrationMBAC(DataModules *data, QWidget *parent=nullptr, Qt::WindowFlags f=nullptr);
    ~CalibrationMBAC();
    quint16 calcKUKDac(double iGiven, double measured);
    quint16 calcKUKAdc(/*double iGiven,*/double measured, double adc);
    CalibrationCheck *calibrCheck;
    quint16 getGivenValue(int);

signals:



private slots:
    void execFormCheck();
    void getClick(int id);
    void setButtonDAC1OK();
    void setButtonDAC2OK();
    void setButtonDAC1Write();
    void setButtonDAC2Write();
    void setButtonADC1OK();
    void setButtonADC2OK();
    void setButtonADC1Write();
    void setButtonADC2Write();
    void getClickSpinBoxDAC1(int id);
    void getClickSpinBoxDAC2(int id);
    void setButtonResetAllDAC1();
    void setButtonResetAllDAC2();
    void getClickSpinBoxADC1(int id);
    void getClickSpinBoxADC2(int id);
    void setButtonResetAllADC1();
    void setButtonResetAllADC2();

private:
    //Change format of string/////////////////////////////////////
   void setFormatLineEdit(QLineEdit *qle, QString nmfont, int size,
                           int bold, Qt::Alignment align, int len, int width , bool clbtn);
   //Change format of SpinBox/////////////////////////////////////
    void setFormatSpinBox(QSpinBox *sb, int bold, int min, int max);
    bool getMessageFromOkBtn(QLineEdit *obj);
    void  fieldClearingDac(QLineEdit *currVal, QSpinBox *kukKo, QLineEdit *changeKo, QLineEdit *maVal);

    QButtonGroup *btnGroupDAC1;
    QButtonGroup *btnGroupDAC2;
    QButtonGroup *btnGroupADC1;
    QButtonGroup *btnGroupADC2;

private:
    DataModules *modul;
    QList<SPort*> sPorts;
    void getRadioBtnPush(int idBtn, int* group, int* number, int *ma);
    bool sendCommandDac(SPort *sPort, DataModules* modul, quint8 numDac, quint8 specPar, quint16 datLoHi);
    bool sendCommandDac12(SPort *sPort, DataModules* modul, quint8 command, quint16 datLoHi01, quint16 datLoHi23);
    bool writeDataShortMBAC(SPort *sPort, DataModules* modul, quint8 adr, quint8 cmd);
    bool readUncalibratedADCval(SPort* sPort, DataModules* modul, quint8 command, QLineEdit *qline, QSpinBox *qspin);
    void showDialogInform(QMessageBox::Icon iconType, QString qstr);
    quint8 getKUK(const int selector);

private://----DAC1----------------------//
    void setButtonResetDAC1();
    QGroupBox *initBtnForm();
    QGroupBox *initDAC1();
    QGroupBox *makeGroupBox(QString name);
    QLabel *labCurrValue1;
    QLineEdit *editCurrValue1;
    QPushButton *buttonOK1;
    QLabel *labCalibrKo1;
    QSpinBox *spinBox1;
    QPushButton *buttonWrite1;
    QLabel *labHandCorr1;
    QLineEdit *editHandCorr1;
    QPushButton *buttonReset1;

    QPushButton *btnExecCheckDialog;
    QPushButton *btnExit;
    QGroupBox *createGroupRadioBtn();
    int countDAC1;
    int prevDAC1;
    int idRadioBtn;//id радио кнопки//

private://----DAC2----------------------//
    void setButtonResetDAC2();
    QGroupBox *initDAC2();
    QGroupBox *makeGroupBox2(QString name);
    QLabel *labCurrValue2;
    QLineEdit *editCurrValue2;
    QPushButton *buttonOK2;
    QLabel *labCalibrKo2;
    QSpinBox *spinBox2;
    QPushButton *buttonWrite2;
    QLabel *labHandCorr2;
    QLineEdit *editHandCorr2;
    QPushButton *buttonReset2;
    QGroupBox *createGroupRadioBtn2();
    int countDAC2;
    int prevDAC2;

private://----ADC1----------------------//
    void setButtonResetADC1();
    QGroupBox *initADC1();
    QGroupBox *makeGroupBox1_1(QString name);
    QLabel *labCurrValue3;
    QLineEdit *editCurrValue3;
    QPushButton *buttonOK3;
    QLabel *labAdc3;
    QLineEdit *editAdc3;
    QLabel *labAdcMa3;
    QLabel *labCalibrKo3;
    QSpinBox *spinBox3;
    QPushButton *buttonWrite3;
    QLabel *labHandCorr3;
    QLineEdit *editHandCorr3;
    QPushButton *buttonReset3;
    QGroupBox *createGroupRadioBtn1_1();
    int countDAC3;
    int prevDAC3;


private://----ADC2----------------------//
    void setButtonResetADC2();
    QGroupBox *initADC2();
    QGroupBox *makeGroupBox1_2(QString name);
    QLabel *labCurrValue4;
    QLineEdit *editCurrValue4;
    QPushButton *buttonOK4;
    QLabel *labAdc4;
    QLineEdit *editAdc4;
    QLabel *labAdcMa4;
    QLabel *labCalibrKo4;
    QSpinBox *spinBox4;
    QPushButton *buttonWrite4;
    QLabel *labHandCorr4;
    QLineEdit *editHandCorr4;
    QPushButton *buttonReset4;
    QGroupBox *createGroupRadioBtn1_2();
    int countDAC4;
    int prevDAC4;

};

#endif // CALIBRATIONMBAC_H
