#ifndef TESTTIMER_H
#define TESTTIMER_H

#include <QObject>

#include "worker.h"

class TestTimer : public Worker
{
    Q_OBJECT

public:
    explicit TestTimer(QList<QString> portname, DataModules *modok = 0, DataModules *modtm = 0, QObject *parent = 0);
    ~TestTimer();

private:
    bool writeTModule(QSerialPort *thisPort, int cmd);

signals:

private slots:
    void startTest();
};

#endif // TESTTIMER_H
