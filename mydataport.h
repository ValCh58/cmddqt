#ifndef MYDATAPORT_H
#define MYDATAPORT_H

#include <qobject.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "logfile.h"
#include "protocol.h"


class MyDataPort : public QObject
{
    Q_OBJECT
public:
    explicit MyDataPort(QString portname, QObject *parent = nullptr);
    ~MyDataPort();
    QSerialPort *thisPort;
    void connectPort();
    void disconnectPort();
    QString getPortName() const;
    QSerialPort *getThisPort() const;
    QByteArray RX;
    quint8 TX[14];
    quint8 crc(const quint8 *buf, const int sz);

private:
    QString portName;
    LogFile *logFile;
    bool writeSettingsPort();
    bool resultConnect;
    int WRITE_TIMEOUT;
    int READ_TIMEOUT;
    int wrireByte;
    int readByte;
    bool getDataFromPort(MyDataPort *port, int cmd, bool *flagOK, int type=0);
    void prepareDataTX(MyDataPort *port, quint8 addr, quint8 cmd, int type=0);
    bool preparedDataRead(char addr, int cmd);
    void writeDataAcp(quint8 adrMod, quint8 cmd/*, quint8 numDac, quint8 srvDat, quint8 *low, quint8 *hi*/);
    int getSizeByte(int type, int command);

signals:
    void sigError(QString err);

public slots:
    void writePort(MyDataPort*, int size, int addr, int CMD, bool *flag=nullptr);

};

#endif // MYDATAPORT_H
