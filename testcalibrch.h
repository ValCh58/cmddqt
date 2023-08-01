#ifndef TESTCALIBRCH_H
#define TESTCALIBRCH_H

#include <QObject>
#include "worker.h"

class TestCalibrCh : public Worker
{
    Q_OBJECT

public:
    TestCalibrCh(quint8 ch, QList<QString> portname, DataModules *modok = 0, DataModules *modtm = 0, QObject *parent = 0);
    ~TestCalibrCh();

private:
    bool selectChannel();
    quint8 channel;
    bool writeTmOkModule(QSerialPort *thisPort, DataModules *mod, int cmd);
    bool actionWaiting();
    bool retValError(int val);
    int idxTableView;
    bool readAcpReal(ulong *a);


private slots:
    void startTest();
};

#endif // TESTCALIBRCH_H
