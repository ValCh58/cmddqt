#ifndef CALIBRATEDIALOG_H
#define CALIBRATEDIALOG_H

#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QProgressBar>
#include <QPushButton>
#include <QCheckBox>

#include "myprogressbar.h"
#include "datamodules.h"
#include "sport.h"
#include "calibrationmbac.h"

class CalibrateDialog : public QDialog
{
    Q_OBJECT

public:
    CalibrateDialog(DataModules *data, QWidget * parent = nullptr, Qt::WindowFlags f = nullptr);
    ~CalibrateDialog();



private:
  void init();
  QSpinBox *cntTotalMeasur;
  QSpinBox *cntPeriod;
  QLabel *labAver;//Заголовок столбца средние значения//
  QLabel *labDevi;//Заголовок ср. квадратичные отклонения//
  QLineEdit *txtAcp1Aver;
  QLineEdit *txtAcp1Devi;
  QLineEdit *txtAcp2Aver;
  QLineEdit *txtAcp2Devi;
  QLineEdit *txtAcp3Aver;
  QLineEdit *txtAcp3Devi;
  QLineEdit *txtAcp4Aver;
  QLineEdit *txtAcp4Devi;
  QLineEdit *txtAcp5Aver;
  QLineEdit *txtAcp5Devi;
  QLineEdit *txtAcp6Aver;
  QLineEdit *txtAcp6Devi;
  QLineEdit *txtAcp7Aver;
  QLineEdit *txtAcp7Devi;
  QLineEdit *txtAcp8Aver;
  QLineEdit *txtAcp8Devi;
  MyProgressBar *vProgrBar;
  QPushButton *btMeasur;//Кнопка измерить//
  QGroupBox *createLayoutPgrogressBar();
  QFormLayout *createHLayoutSpinCount();
  QGroupBox   *createGroupAcp1();
  QGroupBox   *createGroupAcp2();

  QGroupBox *createGroupKalibr();
  QRadioButton *btMa4;
  QRadioButton *btMa20;

  QGroupBox *createGroupKanal();
  QRadioButton *btAcp1;
  QRadioButton *btAcp2;
  QRadioButton *btAcp3;
  QRadioButton *btAcp4;
  QRadioButton *btAcp5;
  QRadioButton *btAcp6;
  QRadioButton *btAcp7;
  QRadioButton *btAcp8;

  QVBoxLayout *createGroupBT();
  QPushButton *btWrite;
  QPushButton *btReset;

  QFormLayout *createGroupACP1_4();
  QLineEdit *txtAcp1;
  QLineEdit *txtAcp2;
  QLineEdit *txtAcp3;
  QLineEdit *txtAcp4;

  QFormLayout *createGroupACP5_8();
  QLineEdit *txtAcp5;
  QLineEdit *txtAcp6;
  QLineEdit *txtAcp7;
  QLineEdit *txtAcp8;

  QVBoxLayout *createGroupLoad();
  QCheckBox *chWriteFile;
  QCheckBox *chSlave;
  QPushButton *btControl;
  //QPushButton *btStop;

  QPushButton *btReturn;

  QList<SPort*> sPorts;
  bool flOK;
  int getNumAcp();
  void fillControl(int type);



  uint res[8];//вычисленные (усредненные) значения калибровочных параметров
  uint num_izm;//Количество измерений для калибровочных параметров
  uint  srotkl[8];//Среднеквадратичные отклонения калибровочных параметров
  uint izm[1024][8];//Массив измерений мгновенных значений калибровочных параметров
  uint cIzm[8];//Используется в контрольных измерениях
  DataModules *modul;
  void initData();
  void disabledButton(bool flag);
  void fillFIelds(int type);

  private slots:
  void makeCalibrate();
  void processMeasur(int type);
  void writeCalibrationFactor();
  void resetCalibCoeff();
  void ControlMeasur();

 signals:
  void sigMeasur0(int, DataModules*, bool*, uint*);
  void sigMeasur1(int, DataModules*, bool*, uint*);
  void changeProgress(int);
  void sigWriteCalFactor0(int, int, int, int, uint*);
  void sigWriteCalFactor1(int, int, int, int, uint*);
  void sigResetCalCoeff0(int, int);
  void sigResetCalCoeff1(int, int);
  void sigControlMeasur0(int, int, int, uint*, LogFile*);
  void sigControlMeasur1(int, int, int, uint*, LogFile*);

};

#endif // CALIBRATEDIALOG_H
