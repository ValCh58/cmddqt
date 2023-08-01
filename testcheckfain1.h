#ifndef TESTCHECKFAIN1_H
#define TESTCHECKFAIN1_H

#include <QObject>
#include "worker.h"

class TestCheckFAIN1 : public Worker
{
    Q_OBJECT

public:
    TestCheckFAIN1(quint8 ch, QList<QString> portname, DataModules *modok = 0, DataModules *modtm = 0, QObject *parent = 0);
    ~TestCheckFAIN1();

private:
    bool selectChannel();
    quint8 channel;
    bool writeTmOkModule(QSerialPort *thisPort, DataModules *mod, int cmd);
    bool actionWaiting();
    bool retValError(int val);
    int idxTableView;
    int readAcp(float *a);


private slots:
    void startTest();

};

#endif // TESTCHECKFAIN1_H
