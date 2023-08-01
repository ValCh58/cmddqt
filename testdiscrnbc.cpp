#include "testdiscrnbc.h"
#include <qdebug.h>

TestDiscrNBC::TestDiscrNBC(QList<QString> portname, DataModules *modok, DataModules *modtm, QObject *parent):
                    Worker(portname, modok, modtm, parent)
{
    self_terminate = false;
    user_terminate = true;
    error_terminate = false;
    user_ready = true;
    numError = 0;
}

TestDiscrNBC::~TestDiscrNBC()
{

}


void TestDiscrNBC::startTest()
{
    emit sigPrintMsg("Пожалуйста, установите 30В на трансформаторе.\nНажмите кнопку \"Готов\" для продолжения.",0);
    actionWaiting();
    if(!user_terminate){
        emit sigStateBtn(BTNSTART|BTNCLEAR);
        emit sigPrintMsg("Тест был прерван!",3);
        return;
    }
    if(prepareTest30v()){
       emit sigPrintMsg("Пожалуйста, установите 187В на трансформаторе.\nНажмите кнопку \"Готов\" для продолжения.",0);
       actionWaiting();
       if(!user_terminate){
           emit sigStateBtn(BTNSTART|BTNCLEAR);
           emit sigPrintMsg("Тест был прерван!",3);
           return;
       }
       emit sigPrintMsg("Ждите идет проверка!",1);
       checkSignals();
       finTest();
    }
    emit sigCalcRetInfo(error_terminate, numError);
    if(error_terminate){
       emit sigStopTest();
    }
}


bool TestDiscrNBC::prepareTest30v()
{
    bool resWrite = false;

    writeTmOkModule(portTm, moduleTM, 6);//Вкл.//
    //Заполним весь ТМ РДС//
    uTXbuf = 0xFFFFFFFF;
    memcpy(TX+2, uTX, 8);
    if(writeTmOkModule(portTm, moduleTM, 1 |(moduleTM->getNumInternal().toInt()<<6))){
       return retValError(RESPONS_BAD_TM);
    }
    QThread::msleep(500);
    memset(TX, 0, sizeof(TX));
    //Читаем из ОК НВС//
    if(writeTmOkModule(portOk, moduleOK, 0|(1<<4))){
       return retValError(RESPONS_BAD_OK);
    }
    checkResetZero();
    if(uRXbuf != uTXbuf){//Все не сбросилось в ноль!//
       return retValError(INSTALL_ERROR_ZERO);
    }
    emit sigMakeTableDiscret(16);//Создадим таблицу для сигналов//
    QThread::msleep(50);
    emit sigSetSignalDiscret(0x00000000, uTXbuf,16);//Заполним таблицу//
    //Отключим весь ТМ РДС//
    uTXbuf = 0x00000000;
    memcpy(TX+2, uTX, 8);
    if(writeTmOkModule(portTm, moduleTM, 1 |(moduleTM->getNumInternal().toInt()<<6))){
       return retValError(RESPONS_BAD_TM);
    }
    QThread::msleep(500);
    memset(TX, 0, sizeof(TX));
    resWrite = true;
    return resWrite;
}

bool TestDiscrNBC::retValError(int val)
{
    numError = val;
    error_terminate = true;
    emit sigSetCnt(++passes);
    return false;
}


bool TestDiscrNBC::writeTmOkModule(QSerialPort *thisPort, DataModules *mod, int cmd)
{
    bool flagOK, readOK, retVal = true;

    writePort(thisPort, mod->getAddrDecMod().toInt(), cmd, moduleTM->getTypeModule(), &flagOK);
    if(flagOK){//Передача ОК - ?//
       readOK = getDataFromPort(thisPort, cmd, &flagOK, moduleTM->getTypeModule());
    }
    if(readOK){//ТМ or OK в порядке//
       retVal = false;
    }

    return retVal;
}


void TestDiscrNBC::checkResetZero()
{
    //Проверка сброса в '0'//
    uTXbuf = 0;
    for(int i=0; i < 4; i++){
        uRX[i] = (quint8)RX.at(i+2);
    }
    // Обнулим сиgналы соседнего модуля//
    if (moduleOK->getNumLine().toInt() == 1)
    {
       uRX[2] = 0;
       uRX[3] = 0;
    }else{
         uRX[0] = uRX[2];
         uRX[1] = uRX[3];
         uRX[2] = 0;
         uRX[3] = 0;
    }
}

bool TestDiscrNBC::checkSignals()
{
    bool ret = true;
    int i = 0;
    while(user_terminate && (!error_terminate) /*|| passes <= 2)*/){

         emit sigSetCnt(++passes);
         for(i = 0; i < 16 && user_terminate; i++){
            uTXbuf = (1 << (i*2)) | (1 << (i*2 + 1));
            // Запишем в РДС, проверим что записалось//
            memcpy(TX+2, uTX, 8);
            if(writeTmOkModule(portTm, moduleTM, 1 |(moduleTM->getNumInternal().toInt()<<6))){
               ret = retValError(RESPONS_BAD_TM);
               break;
            }
            QThread::msleep(800);
            //Читаем из КДС/ННС/НВС, проверим что прочиталось//
            memset(TX, 0, sizeof(TX));
            if(writeTmOkModule(portOk, moduleOK, 0|(1<<4))){
               ret = retValError(RESPONS_BAD_OK);
               break;
            }
            //Сравним сигналы//
            checkResetZero();
            //Сигнал обновления таблицы//
            emit sigCheckSignals((1 << i),uRXbuf, i+1);
        }
        emit sigDiscrError();//Обработка результатов//
    }

    return ret;
}

void TestDiscrNBC::finTest()
{
    QSerialPort *thisPort = listPorts.at(0);

    if(thisPort->isOpen()){
       memset(TX, 0, sizeof(TX));
       writePort(thisPort, 15, 1 | (1 << 6), moduleTM->getTypeModule());
       QThread::msleep(100);
       memset(TX, 0, sizeof(TX));
       writeTmOkModule(portTm, moduleTM, 1|(moduleTM->getNumInternal().toInt()<<6));
       QThread::msleep(100);
    }
}


