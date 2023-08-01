#include "testacp.h"
#include <qmath.h>

TestACP::TestACP(quint8 ch, QList<QString> portname, DataModules *modok, DataModules *modtm, QObject *parent):
          Worker(portname, modok, modtm, parent)
{
    self_terminate = false;
    user_terminate = true;
    error_terminate = false;
    user_ready = true;
    numError = 0;
    channel = ch;
}

TestACP::~TestACP()
{

}

bool TestACP::selectChannel()
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

bool TestACP::writeTmOkModule(QSerialPort *thisPort, DataModules *mod, int cmd)
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

bool TestACP::actionWaiting()
{
    float tmp=0.0;
    bool ret=false;
    ulong val[]={0, 0};
    QString color;
    QString s;
    //----------------------------------------//
    emit sigStateBtn(BTNSTOP|BTNREADY);
    while(user_ready&&user_terminate&&!error_terminate){
        memset(TX, 0, sizeof(TX));
        if(writeTmOkModule(portOk, moduleOK, 0|(moduleOK->getNumInternal().toInt()<<6))){
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
        QThread::msleep(200);
    }
    user_ready = true;
    emit sigStateBtn(BTNSTOP);
    return ret;
    //----------------------------------------//
}

int TestACP::readAcp(float *a)
{
    float tmp=0.0;
    int ret = 0;
    memset(TX, 0, sizeof(TX));
    if(writeTmOkModule(portOk, moduleOK, 0|(moduleOK->getNumInternal().toInt()<<6))){
       ret = retValError(RESPONS_BAD_OK);
    }else{
        if(channel==0){
            quint8 tmp2=(quint8)RX[2];
            quint8 tmp3=(quint8)RX[3];
            tmp = (tmp2 + (tmp3<<8));
            a[0] = (tmp * 2.0/1000.0);
        }else if(channel==1){
            quint8 tmp2=(quint8)RX[4];
            quint8 tmp3=(quint8)RX[5];
            tmp = (tmp2 + (tmp3<<8));
            a[1] = (tmp * 2.0/1000.0);
        }
    }
    return ret;//==0 OK!//
}

void TestACP::startTest()
{
    idxTableView = 0;
    float a[2];
    float  pnt2[] = {7.2,10.4,13.6,16.8};//Ma//
    int i=0;
    emit sigSetCnt(++passes);
    QString head = channel==0?"Измерение ACP1":"Измерение ACP2";
    writeTmOkModule(portTm, moduleTM, 6);//Вкл. соседа.////
    if(!selectChannel()){
       retValError(RESPONS_BAD_RDS15);
    }
    for(i=0; i<4 && user_terminate && !error_terminate; i++){
        emit sigPrintMsg(".......................................................................................................",4);
        emit sigPrintMsg("Пожалуйста - установите значение тока "+QString::number(pnt2[i])+" Ma для проверки канала "+QString::number(channel+1),0);
        //Запускается поток измерения тока. Отображаем значения в ячейке создаваемой таблицы.
        emit sigMakeTextTableAcp(head,QString::number(pnt2[i])+" Ma");
        if(actionWaiting()){
            if(user_terminate){
                retValError(RESPONS_BAD_OK);
            }
           break;
        }
        idxTableView++;
        //Проведем измерения
        if(readAcp(a)){
            retValError(RESPONS_BAD_OK);
            break;
        }
        float otkl = fabs(a[channel] - pnt2[i]);
        QString s1, s2;
        emit sigPrintMsg("Измеренное значение тока канала "+QString::number(channel+1)+": "+s1.sprintf("%2.3f", a[channel])+" Ma ,отклонение "+s2.sprintf("%2.3f", otkl)+" Ma.",4);
        if(otkl  > 0.03){
           emit sigPrintMsg("Отклонение: "+s2.sprintf("%2.3f", otkl)+" Ma.",3);
           retValError(ERROR_OTKL);
           break;
        }
        emit sigPrintMsg("Отклонение: "+s2.sprintf("%2.3f", otkl)+" Ma в допуске.",2);
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
    memset(TX, 0, sizeof(TX));
    writePort(listPorts.at(0), 15, 1 | (1 << 6));//Обнулим 15РДС//
    emit sigACPTable(acpReal, channel);
    emit sigCalcRetInfo(error_terminate, numError);
    if(user_terminate){
       emit sigStopTest();
    }
}

bool TestACP::retValError(int val)
{
    numError = val;
    error_terminate = true;
    return false;
}





