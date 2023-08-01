#include "testcalibrch.h"
#include <qmath.h>
#include <QDebug>

TestCalibrCh::TestCalibrCh(quint8 ch, QList<QString> portname, DataModules *modok, DataModules *modtm, QObject *parent):
    Worker(portname, modok, modtm, parent)
{
    self_terminate = false;
    user_terminate = true;
    error_terminate = false;
    user_ready = true;
    numError = 0;
    channel = ch;
}

TestCalibrCh::~TestCalibrCh()
{

}

bool TestCalibrCh::selectChannel()
{
    bool ret;
    QThread::msleep(100);
    memset(TX,0,sizeof(TX));
    if (moduleOK->getTypeModule() < 2)
        TX[2] |= 3 << ((moduleOK->getTypeModule())*4 + channel*2);
    else
        TX[3] |= 3 << ((moduleOK->getTypeModule()-2)*4 + channel*2);

    writePort(listPorts.at(0), 15, 1 | (1 << 6), moduleTM->getTypeModule(), &ret);
    getDataFromPort(listPorts.at(0), 1 | (1 << 6), &ret, moduleTM->getTypeModule());

    emit sigPrintMsg("Ждите - идет выбор канала ...",1);
    QThread::msleep(2000);
    return ret;//true -> данные записаны//
}

bool TestCalibrCh::writeTmOkModule(QSerialPort *thisPort, DataModules *mod, int cmd)
{
    bool flagOK, readOK, retVal = true;

    writePort(thisPort, mod->getAddrDecMod().toInt(), cmd, moduleTM->getTypeModule(), &flagOK);
    QThread::msleep(2);
    if(flagOK){//Передача ОК - ?//
       readOK = getDataFromPort(thisPort, cmd, &flagOK, moduleTM->getTypeModule());
    }
    if(readOK && flagOK){//в порядке//
       retVal = false;
    }

    return retVal;
}

bool TestCalibrCh::actionWaiting()
{
    float tmp=0.0;
    bool ret=false;
    ulong val[]={0, 0};
    QString color;
    QString s;

    emit sigStateBtn(BTNSTOP|BTNREADY);
    QThread::msleep(100);
    while(user_ready&&user_terminate&&!error_terminate){
        memset(TX, 0, sizeof(TX));
        if(writeTmOkModule(portOk, moduleOK, 14 | (3 << 4))){
           ret=true;
           break;
        }
        if(channel==0){
           quint8 tmp2=(quint8)RX[2];
           quint8 tmp3=(quint8)RX[3];
           val[0] = (tmp2 + (tmp3<<8));
           color = ((1000 < val[0]) && (val[0] < 11000))?"#00FF00":"#0000FF";
           tmp = (val[0]*2.0/1000.0);
        }else{
            quint8 tmp2=(quint8)RX[4];
            quint8 tmp3=(quint8)RX[5];
            val[1] = (tmp2 + (tmp3<<8));
            color = ((1000 < val[1]) && (val[1] < 11000))?"#00FF00":"#0000FF";
            tmp = (val[1]*2/1000.0);
        }
        s.sprintf("%2.3f", tmp);
        emit sigUpdateTableAcp(color, s,idxTableView);
        s="";
        QThread::msleep(300);
    }
    user_ready = true;
    emit sigStateBtn(BTNSTOP);
    return ret;
}

bool TestCalibrCh::retValError(int val)
{
    numError = val;
    error_terminate = true;
    return false;
}

bool TestCalibrCh::readAcpReal(ulong *a)
{
    int i = 0;
     int izm[36][2];
     quint8 num_izm = 0;

     for(i = 0; (i < 36) && user_terminate && !error_terminate; i++){
       if (!writeTmOkModule(portOk, moduleOK, 14 | (3 << 4)))
       {
         izm[num_izm][0] = (quint8)RX[2] + ((quint8)RX[3]<<8);
         izm[num_izm][1] = (quint8)RX[4] + ((quint8)RX[5]<<8);
         num_izm++;
       }
       QThread::msleep(100);
     }
     if (num_izm > 0){
       for(i = 0; i < 2; i++)
       {
         a[i] = 0;
         for(int j = 0; j < num_izm; j++)
           a[i] += izm[j][i];
         a[i] = a[i]/num_izm;
       };
       return false;
     }
     else
       return true;
}

void TestCalibrCh::startTest()
{
    idxTableView = 0;
    ulong a[2];
    float  pnt2[] = {4,20};//Ma//
    int i=0;
    emit sigSetCnt(++passes);
    QString head = channel==0?"Калибровка ACP1":"Калибровка ACP2";
    writeTmOkModule(portTm, moduleTM, 6);//Вкл. соседа.////
    if(!selectChannel()){
       retValError(RESPONS_BAD_RDS15);
    }
    for(i=0; i<2 && user_terminate && !error_terminate; i++){
        emit sigPrintMsg(".......................................................................................................",4);
        emit sigPrintMsg("Пожалуйста - установите значение тока "+QString::number(pnt2[i])+" Ma для калибровки канала "+QString::number(channel+1),0);
        //Запускается измерениe тока. Отображаем значения в ячейке создаваемой таблицы.
        emit sigMakeTextTableAcp(head,QString::number(pnt2[i])+" Ma");
        if(actionWaiting()){
           if(user_terminate){
               retValError(RESPONS_BAD_OK);
           }
           break;
        }
        idxTableView++;
        //Проведем измерения
        if(readAcpReal(a)){
            if(user_terminate){
                retValError(RESPONS_BAD_OK);
            }
            break;
        }
        QString s; float tmp = (float)a[channel]*2/1000.0;
        emit sigPrintMsg("Измеренное значение тока канала "+QString::number(channel+1)+": "+s.sprintf("%2.3f", tmp)+" Ma",4);
        TX[2] = channel + 1;
        TX[3] = i;
        TX[4] = a[channel] & 0xFF;
        TX[5] = (a[channel] >> 8) & 0xFF;
        if (writeTmOkModule(portOk, moduleOK, 14 | (5 << 4))){
            retValError(RESPONS_BAD_OK);
            break;
        }
        emit sigPrintMsg("Калибровочный коэффициент записан.",2);
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
    memset(TX, 0, sizeof(TX));
    writePort(listPorts.at(0), 15, 1 | (1 << 6));//Обнулим 15РДС//
    emit sigACPTable(acpCorrect, channel);
    emit sigCalcRetInfo(error_terminate, numError);
    if(user_terminate){
       emit sigStopTest();
    }
}

