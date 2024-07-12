#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QList>


#include "logfile.h"
#include "protocol.h"
#include "datamodules.h"
#include "test.h"

#define CHECK_BUFFERS_BAD  11
#define RESPONS_BAD_OK     4
#define RESPONS_BAD_TM     3
#define RESPONS_BAD_RDS15  5
#define ERROR_OTKL         6
#define ANSWER_OFF_LINE    12
#define NO_ANSWER_ON_LINE  13
#define INSTALL_ERROR_ZERO 22
#define BAD_SIGNAL         23

//Что будет запрашиваться и отображаться!
enum ACPTYPE {
     acpCorrect, //Исправленное за калибровку значение АЦП//
     acpReal,    //Реальное значение АЦП//
     fainReal    //Значение частоты//
};

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QList<QString> portname,DataModules *modok, DataModules *modtm, QObject *parent = 0);
    ~Worker();
    //QList<QSerialPort*> thisPort;//Текущий порт//
    void connectPort(QSerialPort *thisPort);
    void disconnectPort(QSerialPort *thisPort);
    QByteArray RX;
    char TX[14];
    char crc(const char *buf, const int sz);

    bool getSelf_terminate() const;
    void setSelf_terminate(bool value);

    bool getUser_terminate() const;
    void setUser_terminate(bool value);

    bool getError_terminate() const;
    void setError_terminate(bool value);

    bool getUser_ready() const;
    void setUser_ready(bool value);


protected:
    QList<QSerialPort*> listPorts;//Порты линий 1 и 2//
    bool self_terminate;//Признак нормального завершения цикла в потоке
    bool user_terminate;//Завершение цикла пользователем//или кнопка ДА!
    bool error_terminate;//Ошибка исполнения цикла зависание ...//или кнопка НЕТ!
    bool user_ready;//События кнопки ГотоВ//
    DataModules *moduleTM;
    DataModules *moduleOK;
    //QString portName;
    bool writeSettingsPort(QSerialPort *thisPort);
    int wrireByte;
    int readByte;
    int WRITE_TIMEOUT;
    int READ_TIMEOUT;
    LogFile *logFile;
    bool getDataFromPort(QSerialPort *thisPort, int cmd, bool *flagOK, int type=0);
    int prepareDataTX(char addr, int cmd, int type=0);
    bool preparedDataRead(char addr, int cmd);
    int getSizeByte(int type, int command);
    int getSizeByteRead(int type, int command);
    void writePort(QSerialPort *thisPort, int addr, int CMD, int type=0, bool *flag=0);
    bool writeTModule(QSerialPort *thisPort, int cmd);
    virtual bool writeTmOkModule(QSerialPort *thisPort, DataModules *mod, int cmd);
    int numError;
    uint passes;
    QSerialPort *portTm, *portOk;
    void actionWaiting();


private:
    bool ctrlPorts();

signals:
    void sigError(QString err);
    void sigStopTest();
    void sigCalcRetInfo(bool, bool, bool, QByteArray);
    void sigCalcRetInfo(bool, int);
    void sigCalcRetInfo();
    void sigSetCnt(int);//Установка текущего значения цикла//
    void sigPrintMsg(QString, int);//Пeчать сообщения//
    void sigStateBtn(quint8);
    void sigMakeTableDiscret(int);
    void sigSetSignalDiscret(ulong, ulong,int);
    void sigCheckSignals(ulong, ulong, int);
    void sigDiscrError();
    void sigACPTable(int,int);
    void sigMakeTextTableAcp(QString, QString);
    void sigUpdateTableAcp(QString, QString, int);


private slots:
    void onTimeOut();//Тестовая функция!!!
    void terminateTest(bool stop);
    void startTest();

};



#endif // WORKER_H
