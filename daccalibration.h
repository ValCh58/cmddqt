#ifndef DACCALIBRATION_H
#define DACCALIBRATION_H

#include "datamodules.h"
#include "sport.h"

#include <QGroupBox>
#include <QLineEdit>
#include <QObject>
#include <QSpinBox>
#include <QWidget>
#include <QFormLayout>
#include <QRadioButton>
#include <QVBoxLayout>


class DacCalibration : public QWidget
{
    Q_OBJECT
public:
    explicit DacCalibration(QWidget *parent = nullptr);
    ~DacCalibration();

    QLineEdit *getEditCurrValue() const;
    void setEditCurrValue(QLineEdit *value);

    QSpinBox *getSpinBox() const;
    void setSpinBox(QSpinBox *value);

signals:

public slots:
    void init();
private:
    DataModules *modul;
    QList<SPort*> sPorts;
    QLabel *labCurrValue;
    QLineEdit *editCurrValue;
    QPushButton *buttonOK;
    QLabel *labCalibrKo;
    QSpinBox *spinBox;
    QPushButton *buttonWrite;

    QGroupBox *createGroupRadioBtn();
    QGroupBox *makeGroupBox(QString name);
};

#endif // DACCALIBRATION_H
