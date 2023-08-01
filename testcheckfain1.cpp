#include <QtMath>

#include "testcheckfain1.h"

TestCheckFAIN1::TestCheckFAIN1(quint8 ch, QList<QString> portname, DataModules *modok, DataModules *modtm, QObject *parent):
    Worker(portname, modok, modtm, parent)
{
    self_terminate = false;
    user_terminate = true;
    error_terminate = false;
    user_ready = true;
    numError = 0;
    channel = ch;
}

TestCheckFAIN1::~TestCheckFAIN1()
{

}

bool TestCheckFAIN1::selectChannel()
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


bool TestCheckFAIN1::writeTmOkModule(QSerialPort *thisPort, DataModules *mod, int cmd)
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

bool TestCheckFAIN1::actionWaiting()
{
    ulong tmp;
    bool ret=false;
    ulong val[]={0, 0};
    QString color;
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
           color = (val[0] <= 4000)?"#00FF00":"#0000FF";
           tmp=val[0];
        }else{
            quint8 tmp2=(quint8)RX[4];
            quint8 tmp3=(quint8)RX[5];
            val[1] = (tmp2 + (tmp3<<8));
            color = (val[1] <= 4000)?"#00FF00":"#0000FF";
            tmp=val[1];
        }
        emit sigUpdateTableAcp(color, QString::number(tmp),idxTableView);
        QThread::msleep(300);
    }
    user_ready = true;
    emit sigStateBtn(BTNSTOP);
    return ret;
    //----------------------------------------//
}

bool TestCheckFAIN1::retValError(int val)
{
    numError = val;
    error_terminate = true;
    return false;
}

int TestCheckFAIN1::readAcp(float *a)
{
    int ret = 0;
    memset(TX, 0, sizeof(TX));
    if(writeTmOkModule(portOk, moduleOK, 0|(moduleOK->getNumInternal().toInt()<<6))){
       ret = retValError(RESPONS_BAD_OK);
    }else{
          a[0] = ((quint8)RX[2] + ((quint8)RX[3]<<8))*2.0/1000;
          a[1] = ((quint8)RX[4] + ((quint8)RX[5]<<8))*2.0/1000;
    }
    return ret;//==0 OK!//
}

void TestCheckFAIN1::startTest()
{
    idxTableView = 0;
    float a[2];
    ulong r[2];
    int  pnt2[] = {0,800,1600,2400,3200};//Набор частот для теста//
    int i=0;
    emit sigSetCnt(++passes);
    QString head = channel==0?"FAIN1":"FAIN2";
    writeTmOkModule(portTm, moduleTM, 6);//Вкл. соседа.////
    if(!selectChannel()){
       retValError(RESPONS_BAD_RDS15);
    }
    //++++++++++++++++++++++++++++++++++++++++++//
    for(i=0; i<5 && user_terminate && !error_terminate; i++){
        emit sigPrintMsg(".......................................................................................................",4);
        emit sigPrintMsg("Пожалуйста - установите значение частоты "+QString::number(pnt2[i])+" для проверки канала "+QString::number(channel+1),0);
        emit sigPrintMsg("Выставите амплитуду 0.032 В для проверки '0'",0);
        //Запускается поток измерения частоты. Отображаем значения в ячейке создаваемой таблицы.
        emit sigMakeTextTableAcp(head,QString::number(pnt2[i])+" Гц/0.032 В");
        if(actionWaiting()){
           retValError(RESPONS_BAD_OK);
           break;
        }
        idxTableView++;
        //Проведем измерения
        if(readAcp(a)){
            retValError(RESPONS_BAD_OK);
            break;
        }
        //Обработаем данные
        r[0] = qCeil(a[0] * 1000/2);
        r[1] = qCeil(a[1] * 1000/2);
        if(!(r[0]==0 && r[1]==0)){
           retValError(INSTALL_ERROR_ZERO);
           break;
        }

        emit sigPrintMsg("Тест '0' пройден",2);
        if(!selectChannel()){
           retValError(RESPONS_BAD_RDS15);
           break;
        }
        emit sigPrintMsg("Выставите амплитуду 0.1 вольта для проверки значения частоты",0);
        emit sigMakeTextTableAcp(head,QString::number(pnt2[i])+" Гц/0.1 В");
        if(actionWaiting()){
           retValError(RESPONS_BAD_OK);
           break;
        }
        idxTableView++;
        //Проведем измерения//
        if(readAcp(a)){
            retValError(RESPONS_BAD_OK);
            break;
        }
        //Обработаем данные
        r[0] = qCeil(a[0] * 1000/2);
        r[1] = qCeil(a[1] * 1000/2);
        //Обработаем данные
        float otkl = abs((long)(r[channel] - pnt2[i]));
        if(otkl > 10){
           emit sigPrintMsg("Отклонение: "+QString::number(otkl)+" Гц.",3);
           emit sigPrintMsg("FAIN"+QString::number(channel+1)+" частота "+QString::number(r[channel])+" Гц, "+"превышено отклонение на "+QString::number(otkl)+" Гц.",3);
           retValError(ERROR_OTKL);
           break;
        }
        emit sigPrintMsg("Отклонение: "+QString::number(otkl)+" Гц.",2);
        emit sigPrintMsg("FAIN"+QString::number(channel+1)+" частота "+QString::number(r[channel])+" Гц, "+"отклонение "+QString::number(otkl)+" Гц в допуске.",2);
    }
    //++++++++++++++++++++++++++++++++++++++++++//
    memset(TX, 0, sizeof(TX));
    writePort(listPorts.at(0), 15, 1 | (1 << 6));//Обнулим 15РДС//
    emit sigACPTable(fainReal, channel);
    emit sigCalcRetInfo(error_terminate, numError);
    if(user_terminate){
       emit sigStopTest();
    }
}

