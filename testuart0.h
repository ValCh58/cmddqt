#ifndef TESTUART0_H
#define TESTUART0_H

#include <QObject>
#include <QThread>

#include "worker.h"

class TestUART0 : public Worker
{
    Q_OBJECT
public:
    TestUART0(QList<QString> portname, DataModules *modok = 0, DataModules *modtm = 0, QObject *parent = 0);
    ~TestUART0();

private:
    quint8 bufWrCmd[11];//Буфер для записи данных//
    quint8 bufRdCmd[11];//Буфер для чтения данных//
    void randWrite();
    bool checkBuffers();
    uint readRX1OK;
    void cpyRTBuf();
    bool writeTmOkModule(QSerialPort *thisPort, DataModules *mod, int cmd);
    bool onlyWriteRand(QSerialPort *thisPort, DataModules *mod, int cmd);


signals:


private slots:
   void startTest();

};

#endif // TESTUART0_H
