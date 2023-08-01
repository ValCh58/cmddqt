#include <QDebug>

#include "worker.h"
#include "xmldocument.h"


Worker::Worker(QList<QString> portname, DataModules *modok, DataModules *modtm, QObject *parent) : QObject(parent)
{
    moduleOK = modok;
    moduleTM = modtm;
    WRITE_TIMEOUT = 100;
    READ_TIMEOUT  = 100;
    passes = 0;
    memset(TX,0,sizeof(TX));
    listPorts.append(new QSerialPort(this));//Линия 1//
    listPorts.at(0)->setPortName(portname.at(0));
    listPorts.append(new QSerialPort(this));//Линия 2//
    listPorts.at(1)->setPortName(portname.at(1));
    //qDebug()<<"Worker::portName: " << portname.at(0);
    //qDebug()<<"Worker::portName: " << portname.at(1);
    QDir d;
    QString path = d.currentPath()+QDir::separator()+"log"+QDir::separator()+"portTests"+".log";
    logFile = new LogFile(path);
    QThread::usleep(1000);
    ctrlPorts();//Открытие портов//!!!
    //qDebug()<<"Worker ID:"<<QThread::currentThreadId();
}


Worker::~Worker()
{
    disconnectPort(listPorts.at(0));
    //qDebug()<<"Worker::Close Port: "<<listPorts.at(0)->portName()+" is "<<!listPorts.at(0)->isOpen()<<" ID:"<<QThread::currentThreadId();
    disconnectPort(listPorts.at(1));
    //qDebug()<<"Worker::Close Port: "<<listPorts.at(1)->portName()+" is "<<!listPorts.at(1)->isOpen()<<" ID:"<<QThread::currentThreadId();
    delete listPorts.at(1);
    delete listPorts.at(0);
    listPorts.clear();
    delete logFile;
    //qDebug()<<"Worker::Stop: "<<QThread::currentThreadId();
}

void Worker::actionWaiting()
{
     emit sigStateBtn(BTNSTOP|BTNREADY);
     while(user_ready&&user_terminate){
           QThread::msleep(10);
     }
     user_ready = true;
     emit sigStateBtn(BTNSTOP);
}

void Worker::onTimeOut()
{
    qDebug()<<"Worker::onTimeout: "<<QThread::currentThreadId();
}

void Worker::connectPort(QSerialPort *thisPort)
{
    if(thisPort->open(QIODevice::ReadWrite)){
       if(thisPort->clear() && writeSettingsPort(thisPort)){
          logFile->appendString("Порт "+thisPort->portName()+" открыт;");
          //qDebug()<<"Worker::Connect "+thisPort->portName()<<" Open:"<<thisPort->isOpen()<<" ID:"<<QThread::currentThreadId();
       }
    }else{
        disconnectPort(thisPort);
        emit sigError(thisPort->errorString());
        logFile->appendString("Ошибка открытия порта "+thisPort->portName()+";"+thisPort->errorString()+";");
        //qDebug()<<"Worker::ERROR Connect: "+thisPort->portName()<<QThread::currentThreadId();
    }
}

void Worker::disconnectPort(QSerialPort *thisPort)
{
    if(thisPort->isOpen()){
       thisPort->close();
       logFile->appendString("Порт "+thisPort->portName()+" закрыт;");
       //qDebug()<<"Worker::Disconnect: "+thisPort->portName()<<QThread::currentThreadId();
    }
}

char Worker::crc(const char *buf, const int sz)
{
    int sum = 0;

    for(int i = 0; i < sz; i++)
        sum += buf[i];

    return sum;
}
bool Worker::getSelf_terminate() const
{
    return self_terminate;
}

void Worker::setSelf_terminate(bool value)
{
    self_terminate = value;
}
bool Worker::getUser_terminate() const
{
    return user_terminate;
}

void Worker::setUser_terminate(bool value)
{
    user_terminate = value;
}
bool Worker::getError_terminate() const
{
    return error_terminate;
}

void Worker::setError_terminate(bool value)
{
    error_terminate = value;
}
bool Worker::getUser_ready() const
{
    return user_ready;
}

void Worker::setUser_ready(bool value)
{
    user_ready = value;
}


void Worker::terminateTest(bool stop)
{
    setSelf_terminate(stop);
}

void Worker::startTest()
{

}

bool Worker::writeSettingsPort(QSerialPort *thisPort)
{
    bool result = false;

    QSerialPort::BaudRate bod = (QSerialPort::BaudRate)XmlDocument::cfgMap.value("BitSec");
    if(thisPort->setBaudRate(bod)
            && thisPort->setDataBits((QSerialPort::DataBits)XmlDocument::cfgMap.value("ByteSize"))
            && thisPort->setParity((QSerialPort::Parity)XmlDocument::cfgMap.value("Parity"))
            && thisPort->setStopBits((QSerialPort::StopBits)XmlDocument::cfgMap.value("StopBit"))){
       result = true;
       //qDebug()<<"Порт "<<thisPort->portName()<<" настроен";
    }
    return result;
}

bool Worker::getDataFromPort(QSerialPort *thisPort, int cmd, bool *flagOK, int type)
{
   if(thisPort->waitForReadyRead(READ_TIMEOUT)){
      RX.clear();
      qint64 cntByte = thisPort->bytesAvailable();//Доступно байт для чтения//
      RX = thisPort->readAll();
      logFile->appendString("Чтение "+ thisPort->portName() +";"+logFile->getHexFromBuf(RX.data(),cntByte));
      int sz = getSizeByteRead(type, cmd);//Сколько байт должны получить//
      if(sz != cntByte){
         logFile->appendString("Ошибка чтения "+ thisPort->portName() +";"+" Получено бaйт "+QString::number(cntByte)+" вместо "+QString::number(sz));
         *flagOK = false;
      }else if((char)RX[sz-1] != crc(RX,sz-1)){
         logFile->appendString("Ошибка чтения "+ thisPort->portName() +";"+"Неправильная контрольная сумма!");
         *flagOK = false;
      }else if((quint8)RX[0] != (TX[0] | 0x80)){
         logFile->appendString("Ошибка чтения "+ thisPort->portName() +";"+"Пришел неправильный ответ!");
         *flagOK = false;
      }else{
          *flagOK = true;
      }
      return true;
   }else{
      return false;
   }
}

//Подготовка данных к передаче.//////////////////////////////////////////////////////////////////////////////////////
int Worker::prepareDataTX(char addr, int cmd, int type)
{
    int sz = getSizeByte(type, cmd);

    if(sz){
       TX[0] = addr;
       TX[1] = cmd;
       TX[sz-1] = crc(TX, sz-1);
    }
    return sz;
}//================================================================================================================//

int Worker::getSizeByte(int type, int command)
{
    int sz = kvkSIZ0[command] & 0x0F;
    if(type == 4)
       sz = kvkSIZ01[command] & 0x0F;
    return sz;
}

int Worker::getSizeByteRead(int type, int command)
{
    int sz = kvkSIZ0[command] >> 4;
    if(type == 4)
       sz = kvkSIZ01[command] >> 4;
    return sz;
}


//Подготовка данных к приему.////////////////////////////////////////////////////////////////////////////////////////
bool Worker::preparedDataRead(char addr, int cmd)
{
    int sz = kvkSIZ0[cmd] >> 4;
    if ((sz) && ((addr != 0x31) || (cmd == 0x1E) || (cmd == 0x2E)))
        return true;
    return false;
}//================================================================================================================//

void Worker::writePort(QSerialPort *thisPort, int addr, int CMD, int type, bool *flag)
{
    int size = prepareDataTX(addr, CMD, type);//Подготовка данных

    thisPort->write(TX,size);

    if(thisPort->waitForBytesWritten(WRITE_TIMEOUT)){
        if(flag){*flag = true;}
       logFile->appendString("Запись "+thisPort->portName()+";"+logFile->getHexFromBuf(TX,size));
    }else{
        if(flag){*flag = false;}
       emit sigError(thisPort->errorString());
       logFile->appendString("Ошибка записи в порт "+thisPort->portName()+"; "+thisPort->errorString()+"; ");
    }
}

bool Worker::writeTModule(QSerialPort *thisPort, int cmd){
    bool flagOK;
    int addrModule;
    int type;

    addrModule = moduleTM->getAddrDecMod().toInt();
    type = moduleTM->getTypeModule();
    memset(TX,0,sizeof(TX));
    writePort(thisPort, addrModule, cmd, type, &flagOK);
    return flagOK;
}

bool Worker::writeTmOkModule(QSerialPort *thisPort, DataModules *mod, int cmd){return false;}

//Настройка и открытие портов==============================================================================//
bool Worker::ctrlPorts()
{
    bool retConnect = false;
    if(moduleTM && moduleOK){
       if(moduleTM->getNumLine().toInt()-1 == moduleOK->getNumLine().toInt()-1){//Два модуля на одной линии//
           portTm = portOk = listPorts.at(moduleTM->getNumLine().toInt()-1);
           connectPort(portTm);
           retConnect = portTm->isOpen();
       }else{
            portTm = listPorts.at(moduleTM->getNumLine().toInt()-1);//Каждый модуль
            portOk = listPorts.at(moduleOK->getNumLine().toInt()-1);//на отдельной линии//
            connectPort(portTm);
            connectPort(portOk);
            retConnect = (portTm->isOpen() && portOk->isOpen());
       }
    }else if(moduleTM && !moduleOK){//Порт ТМ//
        portTm = listPorts.at(moduleTM->getNumLine().toInt()-1);
        connectPort(portTm);
        retConnect = portTm->isOpen();
    }else if(!moduleTM && moduleOK){//Порт ОК//
        portOk = listPorts.at(moduleOK->getNumLine().toInt()-1);
        connectPort(portOk);
        retConnect = portOk->isOpen();
    }
    return retConnect;
}//=======================================================================================================//






