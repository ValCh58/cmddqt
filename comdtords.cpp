#include "comdtords.h"
#include "xmldocument.h"
#include "protocol.h"
#include "discretrds.h"
#include "testrds.h"

#include <QThread>
#include <QDebug>
#include <QMutex>


extern QMutex mutex;
extern QWaitCondition wait;

/**
 * @brief CmdToRds::CmdToRds
 * @param portname
 * @param rds
 * @param parent
 */
CmdToRds::CmdToRds(QString portname, QList<discretRDS *> rds, QObject *parent) : QObject(parent)

{
    cnt=0;//счетчик выбора модуля//
    rdsLine = rds;
    rdsSize = rdsLine.size();//Количество элементов списка//
    WRITE_TIMEOUT = 50;
    READ_TIMEOUT  = 50;
    port = new QSerialPort(this);
    port->setPortName(portname);
    memset(TX,0,sizeof(TX));
    RX.fill(0);
    ctrlPorts();
    startTest=true;

}

/**
 * @brief CmdToRds::~CmdToRds
 */
CmdToRds::~CmdToRds()
{
   disconnectPort(port);
}

/**
 * @brief CmdToRds::ctrlPorts
 * @return
 */
bool CmdToRds::ctrlPorts()
{
    bool retConnect = false;
    connectPort(port);
    return retConnect = port->isOpen();
}

/**
 * @brief CmdToRds::connectPort
 * @param thisPort
 */
void CmdToRds::connectPort(QSerialPort *thisPort)
{
    if(thisPort->open(QIODevice::ReadWrite)){
       if(!(thisPort->clear()&&writeSettingsPort(thisPort))){
            emit sigError(thisPort->errorString());
       }
    }else{
        disconnectPort(thisPort);
        emit sigError(thisPort->errorString());
        //qDebug()<<"Worker::ERROR Connect: "+thisPort->portName()<<QThread::currentThreadId();
    }
}

/**
 * @brief CmdToRds::disconnectPort
 * @param thisPort
 */
void CmdToRds::disconnectPort(QSerialPort *thisPort)
{
    if(thisPort->isOpen()){
       thisPort->close();
       //qDebug()<<"Worker::Disconnect: "+thisPort->portName()<<QThread::currentThreadId();
    }
}

/**
 * @brief CmdToRds::writeSettingsPort
 * @param thisPort
 * @return
 */
bool CmdToRds::writeSettingsPort(QSerialPort *thisPort)
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

/**
 * @brief CmdToRds::crc
 * @param buf
 * @param sz
 * @return
 */
char CmdToRds::crc(const char *buf, const int sz)
{
    int sum = 0;
    for(int i = 0; i < sz; i++)
        sum += buf[i];
    return sum;
}

//Подготовка данных к передаче.//////////////////////////////////////
/**
 * @brief CmdToRds::prepareDataTX
 * @param addr
 * @param cmd
 * @param type
 * @return
 */
int CmdToRds::prepareDataTX(char addr, int cmd, int type)
{
    int sz = getSizeByte(type, cmd);

    if(sz){
       TX[0] = addr;
       TX[1] = cmd;
       TX[sz-1] = crc(TX, sz-1);
    }
    return sz;
}

/**
 * @brief CmdToRds::getSizeByte
 * @param type
 * @param command
 * @return
 */
int CmdToRds::getSizeByte(int type, int command)
{
    int sz = kvkSIZ0[command] & 0x0F;
    if(type == 4)
       sz = kvkSIZ01[command] & 0x0F;
    return sz;
}

//Подготовка данных к приему./////////////////////////////////////////
/**
 * @brief CmdToRds::preparedDataRead
 * @param addr
 * @param cmd
 * @return
 */
bool CmdToRds::preparedDataRead(char addr, int cmd)
{
    int sz = kvkSIZ0[cmd] >> 4;
    if ((sz) && ((addr != 0x31) || (cmd == 0x1E) || (cmd == 0x2E)))
        return true;
    return false;
}

int CmdToRds::getSizeByteRead(int type, int command)
{
    int sz = kvkSIZ0[command] >> 4;
    if(type == 4)
       sz = kvkSIZ01[command] >> 4;
    return sz;
}

/**
 * @brief CmdToRds::getHexFromBuf
 * @param buf
 * @param size
 * @return
 */
QString CmdToRds::getHexFromBuf(char *buf, int size)
{
    QString str;
    QTextStream ts(&str);
    ts.setIntegerBase(16);
    ts.setFieldWidth(2);
    ts.setPadChar('0');
    for(int i=0; i<size; i++){
        ts<<(quint8)buf[i]<<"  ";
    }
    return str;
}

//Из порта////////////////////////////////////////////////////////////////////////////
/**
 * @brief CmdToRds::getDataFromPort
 * @param thisPort
 * @param cmd
 * @param flagOK
 * @param type
 * @return
 */
bool CmdToRds::getDataFromPort(QSerialPort *thisPort, int cmd, bool *flagOK, int type)
{
   if(thisPort->waitForReadyRead(READ_TIMEOUT)){
      RX.clear();
      qint64 cntByte = thisPort->bytesAvailable();//Доступно байт для чтения//
      RX = thisPort->readAll();
      //qDebug()<<"Data from Port "<<thisPort->portName()<<" "<<getHexFromBuf(RX.data(),cntByte)<<" idThread:"<<QThread::currentThreadId();
      int sz = getSizeByteRead(type, cmd);//Сколько байт должны получить//
      if(sz != cntByte){
         //qDebug()<<"Ошибка чтения "<<thisPort->portName()<<" Получено бaйт "<<QString::number(cntByte)<<" вместо "<<QString::number(sz);
         *flagOK = false;
      }else if((char)RX[sz-1] != crc(RX,sz-1)){
         //qDebug()<<"Ошибка чтения "<<thisPort->portName()<<"Неправильная контрольная сумма!";
         *flagOK = false;
      }else if((quint8)RX[0] != (TX[0] | 0x80)){
         //qDebug()<<"Ошибка чтения "<<thisPort->portName()<<"Пришел неправильный ответ!";
         *flagOK = false;
      }else{
          *flagOK = true;
      }
      return true;
   }else{
      return false;
   }
}

// В порт //////////////////////////////////////////////////////////////////////////////
/**
 * @brief CmdToRds::writePort
 * @param thisPort
 * @param addr
 * @param CMD
 * @param type
 * @param flag
 */
void CmdToRds::writePort(QSerialPort *thisPort, int addr, int CMD, int type, bool *flag)
{
    int size = prepareDataTX(addr, CMD, type);//Подготовка данных

    thisPort->write(TX,size);

    if(thisPort->waitForBytesWritten(WRITE_TIMEOUT)){
        if(flag){*flag = true;}
        //qDebug()<<"To port:"<<thisPort->portName()<<" byte:"<<getHexFromBuf(TX,size)<<" idThread:"<<QThread::currentThreadId();
    }else{
        if(flag){*flag = false;}
       emit sigError(thisPort->errorString());
       //qDebug()<<"Ошибка записи в порт "<<thisPort->portName()<<" "<<thisPort->errorString();
    }
}

/**
 * @brief CmdToRds::commandToRDSNNC
 * @return
 */
bool CmdToRds::commandToRDSNNC()
{
    if(cnt < rdsLine.size()){
        discretRDS *d = rdsLine.at(cnt);
        for(discretRDS *d : rdsLine){
            disconnect(this, SIGNAL(sigSetIndColor(quint32)), d, SLOT(setIndItemRDS(quint32)));
        }
        connect(this, SIGNAL(sigSetIndColor(quint32)), d, SLOT(setIndItemRDS(quint32)));
        uTXbuf = d->getComWord();
        //qDebug()<<"ComWord to port:"<<uTXbuf<<" idThread:"<<QThread::currentThreadId();
        DataModules m = d->getModule();
        int line = d->getLineNNC();//Внутренняя линия//
        //qDebug()<<"line:"<<line<<" "<<QThread::currentThreadId();
        DataModules n = d->getModNNC();//Модуль ННС//

        if(startTest){
           writeModule(port, &m, 6,d ,true);//Включить соседа//
           startTest=false;
        }

        mutex.lock();
        memcpy(TX+2,uTX,4);//Передача состояния кнопок///////////////////////////////////////////////
        bool val = writeModule(port, &m,  1 | (m.getNumInternal().toInt() << 6),d ,true);//Запишем в РДС//
        mutex.unlock();

        if(!val){
            //Чтение состояния кнопок////////////////////////////////////////////////////////////////////////
            int cmd = 0 | (line << 4);
            if(!n.getAddrDecMod().isEmpty()){//Читаем из ННС//
                if(m.getNumLine().toInt() != n.getNumLine().toInt()){
                    //qDebug()<<"Прочитаем из ННС на другом ком порту"<<QThread::currentThreadId();
                    emit sigReqPrt(&n, cmd, d);//Прочитаем из ННС на другом ком порту//
                }else{

                    mutex.lock();
                    bool nnc = writeModule(port, &n,  cmd,d ,false);
                    mutex.unlock();

                    if(!nnc){//Прочитаем из NNC//
                        fromRxToUrx();
                        emit sigSetIndColor(uRXbuf);//Передача прочитанного слова от РДС в discretRDS//
                    }else{
                        //qDebug()<<"Ошибка передачи прочитанного слова от NNC в discretRDS!";
                    }
                }

            }else{//Читаем из РДС//
                bool rds = writeModule(port, &m,  0 | (m.getNumInternal().toInt() << 4),d ,false);
                if(!rds){//Прочитаем из РДС//
                    fromRxToUrx();
                    emit sigSetIndColor(uRXbuf);//Передача прочитанного слова от РДС в discretRDS//
                }else{
                    //qDebug()<<"Ошибка передачи прочитанного слова от РДС в discretRDS!";
                }
            }
        }

        //disconnect(this, SIGNAL(sigSetIndColor(quint32)), d, SLOT(setIndItemRDS(quint32)));

    }

    cnt++;
    if(cnt >= rdsLine.size())
       cnt = 0;

    return true;
}

/**
 * @brief CmdToRds::dataToTX
 * @param data
 */
void CmdToRds::dataToTX(ulong data)
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

/**
 * @brief CmdToRds::fromRxToUrx
 */
void CmdToRds::fromRxToUrx()
{
   for(int i=0; i<4; i++){
       uRX[i] = (quint8)RX[i+2];
   }
}

/**
 * @brief CmdToRds::writeSin
 */
void CmdToRds::writeSin()
{
    //qDebug()<<"Sin Begin"<<QThread::currentThreadId();
    writePort(port, 31, 2);
    QThread::msleep(4);
}

/**
 * @brief CmdToRds::startWriteToRDS
 */
void CmdToRds::startWriteToRDS()
{
    writeSin();
    commandToRDSNNC();
}

/**
 * @brief CmdToRds::testTimer
 */
void CmdToRds::testTimer()
{
    ;//qDebug()<<"TestTimer Sin:";
}

/**
 * @brief CmdToRds::slReqPort
 * @param mod
 * @param cmd
 * @param rds
 */
void CmdToRds::slReqPort(DataModules *mod, int cmd, discretRDS *rds)
{
    mutex.lock();
    bool ret=writeModule(port, mod,  cmd,rds ,false);
    mutex.unlock();

    if(!ret){
        fromRxToUrx();
        emit sigSendWord(uRXbuf);//Передача прочитанного слова от РДС в discretRDS//
    }else{
        //qDebug()<<"Ошибка передачи прочитанного слова от РДС в discretRDS!";
    }

}

/**
 * @brief CmdToRds::slSendWord
 * @param wrd
 */
void CmdToRds::slSendWord(ulong wrd)
{
     emit sigSetIndColor(wrd);
     //qDebug()<<"ComWord from port:"<<wrd<<" idThread:"<<QThread::currentThreadId();
}

/**
 * @brief CmdToRds::writeModule
 * @param thisPort
 * @param mod
 * @param cmd
 * @param rds
 * @param rw
 * @return
 */
bool CmdToRds::writeModule(QSerialPort *thisPort, DataModules *mod, int cmd , discretRDS *rds, bool rw){

    bool flagOK, readOK, retVal = true;

    writePort(thisPort, mod->getAddrDecMod().toInt(), cmd, mod->getTypeModule(), &flagOK);
    if(flagOK){//Передача ОК - ?//
       readOK = getDataFromPort(thisPort, cmd, &flagOK, mod->getTypeModule());
    }
    if(readOK){
       if(rds){
          if(rw){//в счетчики на форме//
             rds->getTxtWrite()->setText(QString::number(rds->getTxtWrite()->text().toInt()+1));//write!//
          }
          else{
             rds->getTxtRead()->setText(QString::number(rds->getTxtRead()->text().toInt()+1));//read!//
          }
       }
       retVal = false;
    }

    return retVal;
}

