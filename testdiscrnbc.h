#ifndef TESTDISCRNBC_H
#define TESTDISCRNBC_H

#include <QObject>
#include "test.h"
#include "worker.h"

class TestDiscrNBC : public Worker
{
    Q_OBJECT

public:
    TestDiscrNBC(QList<QString> portname, DataModules *modok = 0, DataModules *modtm = 0, QObject *parent = 0);
    ~TestDiscrNBC();

private:
    // Исходящий буфер
       union
       {
         quint8  uTX[8];
         ulong uTXbuf;
       };
       //Входящий буфер
       union
       {
         quint8  uRX[8];
         ulong uRXbuf;
       };
       bool prepareTest30v();
       bool writeTmOkModule(QSerialPort *thisPort, DataModules *mod, int cmd);
       void checkResetZero();
       bool checkSignals();
       void finTest();
       bool retValError(int val);

private slots:
       void startTest();


signals:


};

#endif // TESTDISCRNBC_H
