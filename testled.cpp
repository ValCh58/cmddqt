#include "testled.h"


TestLed::TestLed(int N, QList<QString> portname, DataModules *modok, DataModules *modtm, QObject *parent):Worker(portname,modok, modtm, parent)
{
    user_terminate = true;
    error_terminate = true;
    cmdN = N;
}

TestLed::~TestLed()
{

}

bool TestLed::writeTModule(QSerialPort *thisPort, int cmd)
{

    int addrModule = moduleTM->getAddrDecMod().toInt();//Адрес модуля//
    int type = moduleTM->getTypeModule();
    bool flagOK;

    memset(TX,0,sizeof(TX));
    TX[3] = cmdN;
    writePort(thisPort, addrModule, cmd, type, &flagOK);

    return flagOK;
}

void TestLed::startTest()
{
    //QSerialPort *port = listPorts.at(moduleTM->getNumLine().toInt()-1);
    //connectPort(port);
    quint8 cmd = (15 | (3 << 4));//Команда//
    QThread::msleep(5);
    writeTModule(portTm, cmd);//Команда управления лампами N//
    while(user_terminate && error_terminate){
        QThread::msleep(100);
    }
    emit sigCalcRetInfo();
    emit sigStopTest();

}

