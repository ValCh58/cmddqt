#ifndef CHECKKNOAWO_H
#define CHECKKNOAWO_H

#include <QObject>
#include "worker.h"

class CheckKnoAwo : public Worker
{
    Q_OBJECT

public:
    CheckKnoAwo(QList<QString> portname, DataModules *modok = 0, DataModules *modtm = 0, QObject *parent = 0);
    ~CheckKnoAwo();

private:
    bool writeTModule(QSerialPort *thisPort, int cmd);
    quint8 bufWrCmd[11];//Буфер для записи данных//
    quint8 bufRdCmd[11];//Буфер для чтения данных//
    uint readRX1OK;
    void cpyRTBuf();

private slots:
    void startTest(uint *result);
};

#endif // CHECKKNOAWO_H
