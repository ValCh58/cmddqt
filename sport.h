#ifndef SPORT_H
#define SPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "logfile.h"
#include "protocol.h"
#include "datamodules.h"

quint16 convertByteToInt16(quint8 d0, quint8 d1);

class SPort : public QObject
{
    Q_OBJECT
public:
    explicit SPort(QString portname, QObject *parent = nullptr);
    ~SPort();
     QSerialPort *thisPort;
     void connectPort();
     void disconnectPort();
     QString getPortName() const;
     QByteArray RX;
     char TX[14];
     char crc(const char *buf, const int sz);
     QSerialPort *getThisPort() const;
     bool writeDataAcpMBAC(quint8 numDac, quint8 specPar, quint8 adr, quint8 cmd, quint8 low, quint8 hi);
     bool writeDataAcp12MBAC(quint8 adr, quint8 cmd, quint8 low0, quint8 hi1, quint8 low2, quint8 hi3);
     bool writeSPort(quint8 tr[], const quint8 size);
     bool getDataFromPortMBAC(quint8 adr, quint8 sz);



private:
     QString portName;
     bool writeSettingsPort();
     int wrireByte;
    int readByte;
    bool resultConnect;
    int WRITE_TIMEOUT;
    int READ_TIMEOUT;
    LogFile *logFile;
    bool getDataFromPort(SPort *port, int cmd, bool *flagOK, int type=0);
    void prepareDataTX(SPort *port, char addr, int cmd, int type=0);
    bool preparedDataRead(char addr, int cmd);
    void readAcp(int cmd, int addr, bool *flagOk,int type=0);
    int getSizeByte(int type, int command);
    void ResetCalCoeff(int acp, int acpType, int p4, int p5, int cmd, int type, int adr);
    void getDataAcp_1_8(int adr, int cmd, int type, uint *low, uint *hi);
    void getDataAcp_1_2(int adr, int cmd, int type, uint *low, uint *hi);
    void writeDataAcp_1_8(int adr, int cmd, int type, uint *low, uint *hi);
    void writeDataAcp(int adr, int cmd, int type, uint *low, uint *hi);




signals:
    void sigError(QString err);
    void dataReady(QByteArray RX);
    void sigMeasurOK(int type);


public slots:
    void writePort(SPort*, int size, int addr, int CMD, int type=0, bool *flag=nullptr);
    QByteArray readPortAll(SPort* port);
    void readPort(SPort* port, qint64 size);
    void handDataSearchModules(SPort *port, int line, int addr, int cmd);
    void detectModule(SPort *port, int cmd, int addr);
    void makeMeasur(int cntMeasur, DataModules *data, bool *ok, uint *izmer);
    void makeWriteCalFactor(int type, int ma20_40, int numCh, int adr, uint *izmer);
    void ResetCalCoeff(int type, int addr);
    void controlMeasur(int func, int type, int addr, uint *izm, LogFile *log);
    void readSettingsAcp(DataModules *data, uint *low, uint *hi);
    void writeSettingsAcp(DataModules *data, uint *low, uint *hi);
    void ReadRateConst(DataModules *data, uint *low, uint *hi);
    void WriteRateConst(DataModules*data, uint*low, uint*hi);
    void saveSettingsModule(DataModules*data, quint8 adr, quint8 type, quint8 aver);
    void detectModuleTmOk(SPort *port,int cmd,int addr,bool *flag);
    void writeStartTestTM(SPort *port, int cmd, int addr, int N);//Выдача команды в ТМ//

};

#endif // SPORT_H
