#include <QtGlobal>
#include <QDebug>

#include "testuart1.h"


TestUART1::TestUART1(QList<QString> portname, DataModules *modok, DataModules *modtm, QObject *parent):Worker(portname, modok, modtm, parent)
{
    self_terminate = false;
    user_terminate = true;
    error_terminate = false;
    numError = 0;
}

TestUART1::~TestUART1()
{

}

//Старт теста========================//
void TestUART1::startTest()
{
    int cnt = 0;

    qsrand(0xFF);
    while(!self_terminate && (user_terminate || passes < 100) && !error_terminate){
        QThread::msleep(5);
        passes++;
        if(writeTModule(portTm, 15 | (4 << 4))){
           error_terminate = true;
           break;//emit sigStopTest();//break;
        }
        emit sigSetCnt(++cnt);
        //qDebug()<<"cnt:"<<cnt++;
    }
    emit sigCalcRetInfo(error_terminate, numError);
    if(user_terminate){
       emit sigStopTest();
    }
}

//Запись данных в ТМ=================//
bool TestUART1::writeTModule(QSerialPort *thisPort, int cmd)
{
    bool flagOK, readOK, retVal = true;

    randWrite();
    memcpy(TX+2, bufWrCmd, 8);//Запишем данные в буфер передачи//
    writePort(thisPort, moduleTM->getAddrDecMod().toInt(), cmd, moduleTM->getTypeModule(), &flagOK);
    if(flagOK){//Передача ОК - ?//
       readOK = getDataFromPort(thisPort, cmd, &flagOK, moduleTM->getTypeModule());
    }
    if(readOK){//ТМ в порядке//
       cpyRTBuf();
       readRX1OK = bufRdCmd[1];//Равны//
       if(!(readRX1OK & 0X0F)){
          if(!checkBuffers()){//Сравним данные приема и передачи//
             retVal = false;
          }else{
            numError = CHECK_BUFFERS_BAD;
            retVal = true;
            //qDebug()<<"Ошибка ответного кадра OK";
          }
       }else{
         numError = RESPONS_BAD_OK;
         retVal = true;
         //qDebug()<<"Нет ответа ОК";
       }
    }else{
        numError = RESPONS_BAD_TM;
        retVal = true;
        //qDebug()<<"Нет ответа ТМ";
    }

    return retVal;
}

//Запись сген. случайных чисел в буфер//
void TestUART1::randWrite()
{
    //qsrand(0xFF);
    for(int i = 0; i < 8; i++){
        bufWrCmd[i] = qrand();
    }
}

//Сравнение данных передачи и приема==//
bool TestUART1::checkBuffers()
{
    bool ret = (memcmp(bufWrCmd, bufRdCmd+2, sizeof(bufRdCmd)-3));
    memset(bufRdCmd, 0, 11 );
    memset(bufWrCmd, 0, 11 );
    return ret;

}

void TestUART1::cpyRTBuf()
{
    for(int i=0; i < RX.size(); i++){
        bufRdCmd[i] = RX.at(i);
    }
}




