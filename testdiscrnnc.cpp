#include "testdiscrnnc.h"

TestDiscrNNC::TestDiscrNNC(ulong par1, ulong par2, QList<QString> portname, DataModules *modok, DataModules *modtm, QObject *parent):
    Worker(portname, modok, modtm, parent)
{
    self_terminate = false;
    user_terminate = true;
    error_terminate = false;
    user_ready = true;
    numError = 0;
    parCmd1 = par1;
    parCmd2 = par2;
}

TestDiscrNNC::~TestDiscrNNC()
{

}

bool TestDiscrNNC::writeTmOkModule(QSerialPort *thisPort, DataModules *mod, int cmd)
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



void TestDiscrNNC::finTest()
{
    QSerialPort *thisPort = listPorts.at(0);

    if(thisPort->isOpen()){
       memset(TX, 0, sizeof(TX));
       writePort(thisPort, 15, 1 | (1 << 6), moduleTM->getTypeModule());
       QThread::msleep(100);
       memset(TX, 0, sizeof(TX));
       writeTmOkModule(portTm, moduleTM, 1|(moduleTM->getNumLine().toInt()<<6));
       QThread::msleep(100);
    }
}

bool TestDiscrNNC::retValError(int val)
{
    numError = val;
    error_terminate = true;
    emit sigSetCnt(++passes);
    return false;
}

bool TestDiscrNNC::setTM15()
{
    bool ret;

    writePort(listPorts.at(0), 15, 1 | (1 << 6), moduleTM->getTypeModule(), &ret);
    ret = getDataFromPort(listPorts.at(0), 1 | (1 << 6), &ret, moduleTM->getTypeModule());
    QThread::msleep(300);
    return ret;
}

void TestDiscrNNC::dataToTX(ulong data)
{
    union{
        ulong val;
        quint8 uVal[4];
    };
    val = data;
    for(int i = 0; i<4; i++){
        TX[i+2]=uVal[i];
    }
}

void TestDiscrNNC::fromRxToUrx()
{
   for(int i=0; i<4; i++){
       uRX[i] = (quint8)RX[i+2];
   }
}

void TestDiscrNNC::startTest()
{
    emit sigPrintMsg("Инициализация теста начата.",1);
    writeTmOkModule(portTm, moduleTM, 6);//Вкл. соседа//
    emit sigMakeTableDiscret(32);//Создадим таблицу для сигналов//
    QThread::msleep(50);
    emit sigSetSignalDiscret(0x00000000, uTXbuf,32);//Заполним таблицу//
    QThread::msleep(50);
    emit sigPrintMsg("Тест прерывается нажатием кнопки 'Стоп'!",1);
    while(user_terminate && !error_terminate){
          emit sigSetCnt(++passes);
        //Установим выбор теста//
        /*  dataToTX(parCmd1);
          if(!setTM15()){
              retValError(RESPONS_BAD_RDS15);
              break;
          }*/
          if(parCmd2!=0x00000000){
          //Заполним весь ТМ РДС//
          /*   uTXbuf = 0xFFFFFFFF;
             memcpy(TX+2,uTX,4);
             if(writeTmOkModule(portTm, moduleTM, 1 | (moduleTM->getNumInternal().toInt() << 6))){
                 retValError(RESPONS_BAD_TM);
                 break;
             }
             QThread::msleep(300);
          //Читаем из ОК КДС//
             if(writeTmOkModule(portOk, moduleOK, 0 | (moduleOK->getNumInternal().toInt() << 4))){
                 retValError(RESPONS_BAD_OK);
                 break;
             }
             fromRxToUrx();
             //emit sigSetSignalDiscret(0xFFFFFFFF, uRXbuf,32);
             uTXbuf = 0;
             if(uRXbuf != uTXbuf){//Все не сбросилось в ноль!//
                retValError(INSTALL_ERROR_ZERO);
                break;
             }
          //Отключим весь ТМ РДС//
             memset(TX, 0, sizeof(TX));
             if(writeTmOkModule(portTm, moduleTM, 1 | (moduleTM->getNumInternal().toInt() << 6))){
                 retValError(RESPONS_BAD_TM);
                 break;
             }
             QThread::msleep(300);*/
          //Установим выбор теста, включим N25,26,29,30//
             dataToTX(parCmd2);
             if(!setTM15()){
                retValError(RESPONS_BAD_RDS15);
                break;
             }
          }
          for(int i = 0; i < 32 && user_terminate && !error_terminate; i++){
              uTXbuf = (1 << i);
              //Запишем в РДС-проверим что записалось//
              memcpy(TX+2,uTX,4);
              if(writeTmOkModule(portTm, moduleTM, 1 | (moduleTM->getNumInternal().toInt() << 6))){
                 retValError(RESPONS_BAD_TM);
                 break;
              }
              QThread::msleep(300);
              //Читаем из ОК//
              if(writeTmOkModule(portOk, moduleOK, 0 | (moduleOK->getNumInternal().toInt() << 4))){
                 retValError(RESPONS_BAD_OK);
                 break;
              }
              fromRxToUrx();
              emit sigCheckSignals((1 << i),uRXbuf, i+1);
              QThread::msleep(300);
          }
    }

    finTest();
    emit sigCalcRetInfo(error_terminate, numError);
    if(error_terminate){
       emit sigStopTest();
    }
}




