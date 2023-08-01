#include <QDebug>
#include "testuart0.h"


TestUART0::TestUART0(QList<QString> portname, DataModules *modok, DataModules *modtm, QObject *parent):
              Worker(portname, modok, modtm, parent)
{
     user_terminate = true;
     error_terminate = false;
     self_terminate = false;
     numError = 0;
}

TestUART0::~TestUART0()
{

}

bool TestUART0::writeTmOkModule(QSerialPort *thisPort, DataModules *mod, int cmd)
{
    bool flagOK, readOK, retVal = false;

    randWrite();
    memcpy(TX+2, bufWrCmd, 8);//Запишем данные в буфер передачи//
    writePort(thisPort, mod->getAddrDecMod().toInt(), cmd, mod->getTypeModule(), &flagOK);
    if(flagOK){//Передача ОК - ?//
       readOK = getDataFromPort(thisPort, cmd, &flagOK, mod->getTypeModule());
    }
    if(readOK){//OK || ТМ в порядке?//
       cpyRTBuf();
       readRX1OK = bufRdCmd[1];
       if(!checkBuffers()){//Сравним данные приема и передачи//
             retVal = false;
       }else{
            numError = CHECK_BUFFERS_BAD;
            retVal = true;
            //qDebug()<<"Ошибка ответного кадра OK";
       }
    }else{
        numError = ANSWER_OFF_LINE;
        retVal = true;
        //qDebug()<<"Ответ при выключенной линии";
    }
    return retVal;
}

bool TestUART0::onlyWriteRand(QSerialPort *thisPort, DataModules *mod, int cmd)
{
    bool flagOK;
    randWrite();
    memcpy(TX+2, bufWrCmd, 8);//Запишем данные в буфер передачи//
    writePort(thisPort, mod->getAddrDecMod().toInt(), cmd, mod->getTypeModule(), &flagOK);
    return flagOK;
}



void TestUART0::startTest()
{
    int cnt = 0;
    qsrand(0xFF);
    while(!self_terminate && (user_terminate || passes < 5) && !error_terminate){
          passes++;
          writeTModule(portTm, 6);//Вкл.//
          QThread::msleep(400);
          for(int i=0; (i<10) & (user_terminate && !error_terminate); i++){
              emit sigSetCnt(++cnt);//Сигнал счетчика итераций//
              if(writeTmOkModule(portOk, moduleOK, 15 | (4 << 4))){
                 error_terminate = true;
                 emit sigStopTest();
                 break;
              }
          }

          writeTModule(portTm, 4);//Откл.//
          QThread::msleep(400);
          for(int i=0; (i<10) & (user_terminate && !error_terminate); i++){
              emit sigSetCnt(++cnt);//Сигнал счетчика итераций//
              if(!onlyWriteRand(portOk, moduleOK, 15 | (4 << 4))){
                  numError = NO_ANSWER_ON_LINE;
                  error_terminate = true;
                  emit sigStopTest();
                  break;
              }
          }
    }
    emit sigCalcRetInfo(error_terminate, numError);
    if(!error_terminate && user_terminate)
    emit sigStopTest();
}

//Запись сген. случайных чисел в буфер//
void TestUART0::randWrite()
{
    for(int i = 0; i < 8; i++){
        bufWrCmd[i] = qrand();
    }
}

//Сравнение данных передачи и приема==//
bool TestUART0::checkBuffers()
{
    bool ret = (memcmp(bufWrCmd, bufRdCmd+2, sizeof(bufRdCmd)-3));
    memset(bufRdCmd, 0, 11 );
    memset(bufWrCmd, 0, 11 );
    return ret;
}

void TestUART0::cpyRTBuf()
{
    for(int i=0; i < RX.size(); i++){
        bufRdCmd[i] = RX.at(i);
    }
}

