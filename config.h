#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QSerialPortInfo>
#include <QThread>

#include "sport.h"
#include "xmldocument.h"

class config : public QDialog
{
    Q_OBJECT

private:
    void initDataPorts();
    XmlDocument *xmlDoc;
    QStringList listCom;
    void initCmbBox(QComboBox *cmb);
    bool getInfoCom(QString str);
    QList<QSerialPortInfo> infoList;
    QString *helpLine1, *helpLine2;
    void helpInfoSerial(QString com, QString *helpLine);
    QString getNamePort(QString com);
    QComboBox *cmbLine1, *cmbLine2, *cmbBod;
    QThread threade0, threade1;

public:
    config(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~config();
    QComboBox *getCmbBod() const;


private slots:
    void changeLine1(int idx);
    void changeLine2(int idx);
    void changeBod(int idx);
    void changeByteSize(int val);
    void changeParity(int val);
    void changeStopBit(int val);
    void saveConfig();


};

#endif // CONFIG_H
