#ifndef COMDTORDS_H
#define COMDTORDS_H

#include <QString>
#include <QObject>
#include <QSerialPort>
#include <QMap>
#include <QMutex>

#include "datamodules.h"


class discretRDS;

class CmdToRds : public QObject
{
    Q_OBJECT
public:
    explicit CmdToRds(QString portname, QList<discretRDS*> rds, QObject *parent = nullptr);
    ~CmdToRds();


    int getIndexLine() const;
    void setIndexLine(int value);

private:
    QList<discretRDS *> rdsLine;
    int cnt;
    int rdsSize;

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
    int WRITE_TIMEOUT;
    int READ_TIMEOUT;
    QSerialPort* port;
    QByteArray RX;
    char TX[14];
    bool ctrlPorts();
    void connectPort(QSerialPort *thisPort);
    void disconnectPort(QSerialPort *thisPort);
    bool writeSettingsPort(QSerialPort *thisPort);
    void writeSin();
    char crc(const char *buf, const int sz);
    int prepareDataTX(char addr, int cmd, int type);
    int getSizeByteRead(int type, int command);
    bool preparedDataRead(char addr, int cmd);
    int getSizeByte(int type, int command);
    bool getDataFromPort(QSerialPort *thisPort, int cmd, bool *flagOK, int type=0);
    void writePort(QSerialPort *thisPort, int addr, int CMD, int type=0, bool *flag=nullptr);
    bool commandToRDSNNC();
    bool writeModule(QSerialPort *thisPort, DataModules *mod, int cmd, discretRDS *rds=nullptr, bool rw=false);
    QString getHexFromBuf(char *buf, int size);
    void fromRxToUrx();
    void dataToTX(ulong data);
    bool startTest;


signals:
   void sigError(QString s);
   void sigSetIndColor(quint32 wrd);
   void sigReqPrt(DataModules *mod, int cmd, discretRDS *rds);
   void sigSendWord(ulong wrd);


public slots:
    void startWriteToRDS();
    void testTimer();
    void slReqPort(DataModules *mod, int cmd, discretRDS *rds);
    void slSendWord(ulong wrd);


};

#endif // COMDTORDS_H
