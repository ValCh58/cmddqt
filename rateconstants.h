#ifndef RATECONSTANTS_H
#define RATECONSTANTS_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStatusBar>

#include "datamodules.h"
#include "sport.h"
#include "myprogressbar.h"

class RateConstants : public QDialog
{
    Q_OBJECT

public:
    RateConstants(DataModules *data, QWidget *parent=0, Qt::WindowFlags f=0);
    ~RateConstants();

private:
    QLabel *labCh1;
    QLineEdit *editCh1;
    QLabel *labFx1;
    QLineEdit *editFx1;

    QLabel *labCh2;
    QLineEdit *editCh2;
    QLabel *labFx2;
    QLineEdit *editFx2;

    QLabel *labCh3;
    QLineEdit *editCh3;
    QLabel *labFx3;
    QLineEdit *editFx3;

    QLabel *labCh4;
    QLineEdit *editCh4;
    QLabel *labFx4;
    QLineEdit *editFx4;

    QLabel *labCh5;
    QLineEdit *editCh5;
    QLabel *labFx5;
    QLineEdit *editFx5;

    QLabel *labCh6;
    QLineEdit *editCh6;
    QLabel *labFx6;
    QLineEdit *editFx6;

    QLabel *labCh7;
    QLineEdit *editCh7;
    QLabel *labFx7;
    QLineEdit *editFx7;

    QLabel *labCh8;
    QLineEdit *editCh8;
    QLabel *labFx8;
    QLineEdit *editFx8;

    QPushButton *btWrite;
    QPushButton *btCancel;

    MyProgressBar *vProgrBar;
    QStatusBar *sbar;

    void init();
    QGroupBox *makeGroupBox(QString name, QLabel *labCh, QLineEdit *editCh, QLabel *labFx, QLineEdit *editFx);
    DataModules *modul;
    QList<SPort*> sPorts;
    uint arFx[8];
    uint arCh[8];
    void readConst();
    void fillForm();
    void setDisBtn(bool flag);
    void readFromArrSet();


private slots:
   void writeRateConst();

signals:
   void sigReadRateConst0(DataModules*, uint*, uint*);
   void sigReadRateConst1(DataModules*, uint*, uint*);
   void sigWriteRateConst0(DataModules*,uint*,uint*);
   void sigWriteRateConst1(DataModules*,uint*,uint*);

};

#endif // RATECONSTANTS_H
