#ifndef TESTLED_H
#define TESTLED_H

#include <QObject>

#include "worker.h"

class TestLed : public Worker
{
    Q_OBJECT

public:
    TestLed(int N, QList<QString> portname, DataModules *modok = 0, DataModules *modtm = 0, QObject *parent = 0);
    ~TestLed();
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //bool user_terminate;//Завершение цикла пользователем кнопка ДА!
    //bool error_terminate;//Завершение цикла пользователем кнопка НЕТ!
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
private:
    bool writeTModule(QSerialPort *thisPort, int cmd);
    int cmdN;

private slots:
    void startTest();
};

#endif // TESTLED_H
