#include "checkknoawo.h"
#include <QDebug>

CheckKnoAwo::CheckKnoAwo(QList<QString> portname, DataModules *modok, DataModules *modtm, QObject *parent)
            :Worker(portname, modok, modtm, parent)
{
    self_terminate = false;
    user_terminate = true;
    error_terminate = false;
    numError = 0;
    memset(bufWrCmd, 0, sizeof(bufWrCmd));
    memset(bufRdCmd, 0, sizeof(bufRdCmd));
}

CheckKnoAwo::~CheckKnoAwo()
{

}

bool CheckKnoAwo::writeTModule(QSerialPort *thisPort, int cmd)
{
    bool flagOK, readOK, retVal = true;
    int sz = getSizeByte(moduleTM->getTypeModule(), cmd);
    memcpy(TX+2, bufWrCmd, sz-3);
    memset(bufWrCmd, 0, sizeof(bufWrCmd));
    writePort(thisPort, moduleTM->getAddrDecMod().toInt(), cmd, moduleTM->getTypeModule(), &flagOK);
    if(flagOK){//Передача ОК - ?//
       readOK = getDataFromPort(thisPort, cmd, &flagOK, moduleTM->getTypeModule());
    }
    if(readOK){//ТМ в порядке//
       cpyRTBuf();
       readRX1OK = bufRdCmd[1];//Равны//
       if(!(readRX1OK & 0X0F)){
            retVal = false;
            memcpy(bufWrCmd, bufRdCmd, sizeof(bufWrCmd));
       }else{
         numError = RESPONS_BAD_OK;
         retVal = true;
         qDebug()<<"Нет ответа ОК";
       }
    }else{
        numError = RESPONS_BAD_TM;
        retVal = true;
        qDebug()<<"Нет ответа ТМ";
    }

    return retVal;
}

void CheckKnoAwo::startTest(uint *result)
{
    int j = 0;
    memset(bufWrCmd,0,sizeof(bufWrCmd));

    while(!self_terminate && (user_terminate || passes < 50) && !error_terminate){

          emit sigSetCnt(++passes);
          j = 1 - j;
          bufWrCmd[2] = j;
          if(writeTModule(portTm, 15 | (5 << 4))){
             error_terminate = true;
             //emit sigStopTest();
             continue;
          }
          //qDebug()<<"cnt:"<<passes;

          for(int i = 0; i < 4; i++){
              if(bufWrCmd[i+2] != j){
                 int cod = 14 + 4*j;
                 result[cod]++;
              }
          }
    }
    emit sigCalcRetInfo(error_terminate, numError);
    if(user_terminate){
       emit sigStopTest();
    }
}

void CheckKnoAwo::cpyRTBuf()
{
    for(int i=0; i < RX.size(); i++){
        bufRdCmd[i] = RX.at(i);
    }
}
