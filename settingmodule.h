#ifndef SETTINGMODULE_H
#define SETTINGMODULE_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QRadioButton>
#include <QComboBox>

#include "sport.h"
#include "datamodules.h"

class SettingModule : public QDialog
{
    Q_OBJECT

public:
    SettingModule(DataModules *data, QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~SettingModule();

private:
    QList<SPort*> sPorts;
    DataModules *modul;
    void init();
    QGroupBox   *hBox1(QString adr);
    QHBoxLayout *hBox2();
    QGroupBox   *hBox3(QString line);
    QGroupBox   *hBox4(QString acp);
    QHBoxLayout *hBox5();
    QLabel * labAdr;
    QSpinBox *spAdr;
    QLabel *labAdrHex;
    QLineEdit *edAdrHex;
    QCheckBox *cbInv;
    QCheckBox *cbRes;
    QRadioButton *rbLine1;
    QRadioButton *rbLine2;

    QLabel *labAcp1_2;
    QComboBox *spAcp1_2;
    QLabel *labAcp3_4;
    QComboBox *spAcp3_4;
    QLabel *labAcp5_6;
    QComboBox *spAcp5_6;
    QLabel *labAcp7_8;
    QComboBox *spAcp7_8;

    QPushButton *btSave;
    QPushButton *btCancel;
    void initData();

signals:
    void sigSaveParam0(DataModules*, quint8, quint8, quint8);
    void sigSaveParam1(DataModules*, quint8, quint8, quint8);


private slots:
    void changeAdr(int adr);
    void preParam();

};

#endif // SETTINGMODULE_H
