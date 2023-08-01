#ifndef TESTDISCRNNC_H
#define TESTDISCRNNC_H

#include <QObject>
#include "test.h"
#include "worker.h"

class TestDiscrNNC : public Worker
{

    Q_OBJECT

public:
    TestDiscrNNC(ulong par1, ulong par2, QList<QString> portname, DataModules *modok = 0, DataModules *modtm = 0, QObject *parent = 0);
    ~TestDiscrNNC();

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
       bool writeTmOkModule(QSerialPort *thisPort, DataModules *mod, int cmd);
       bool checkSignals();
       void finTest();
       bool retValError(int val);
       bool setTM15();
       void dataToTX(ulong data);
       void fromRxToUrx();
       ulong parCmd1, parCmd2;

private slots:
       void startTest();

};

#endif // TESTDISCRNNC_H
