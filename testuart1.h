#ifndef TESTUART1_H
#define TESTUART1_H

#include "worker.h"
#include "test.h"

#include <QObject>



class TestUART1 : public Worker
{
    Q_OBJECT

public:
    TestUART1(QList<QString> portname, DataModules *modok = 0, DataModules *modtm = 0, QObject *parent = 0);
    ~TestUART1();


private:
    bool writeTModule(QSerialPort *thisPort, int cmd);
    quint8 bufWrCmd[11];//Буфер для записи данных//
    quint8 bufRdCmd[11];//Буфер для чтения данных//
    void randWrite();
    bool checkBuffers();
    uint readRX1OK;
    void cpyRTBuf();



signals:


private slots:
   void startTest();

};

#endif // TESTUART1_H
