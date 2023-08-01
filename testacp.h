#ifndef TESTACP_H
#define TESTACP_H

#include <QObject>
#include "worker.h"

class TestACP : public Worker
{
    Q_OBJECT

public:
    TestACP(quint8 ch, QList<QString> portname, DataModules *modok = 0, DataModules *modtm = 0, QObject *parent = 0);
    ~TestACP();

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

#endif // TESTACP_H
