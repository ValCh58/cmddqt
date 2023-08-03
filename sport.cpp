#include <QMap>
#include <QMessageBox>
#include <QDebug>
#include <QThread>

#include "sport.h"
#include "xmldocument.h"
#include "config.h"


SPort::SPort(QString portname, QObject *parent) : QObject(parent)
{
    WRITE_TIMEOUT = 100;//mlSec
    READ_TIMEOUT  = 100;
    portName = portname;
    thisPort = new QSerialPort(this);
    thisPort->setPortName(portName);
    //Лог файл создадим в каталоге log текущего каталогa программы.//
    //Предварительно проверим существует ли он.//
    QDir d;
    QString path = d.currentPath()+QDir::separator()+"log"+QDir::separator()+"port"+portName.right(4)+".log";
    logFile = new LogFile(path);

}

SPort::~SPort()
{
   disconnectPort();
   delete thisPort;
   delete logFile;
}

QSerialPort *SPort::getThisPort() const
{
    return thisPort;
}

void SPort::connectPort()
{
    if(thisPort->open(QIODevice::ReadWrite)){
        if(thisPort->clear() && writeSettingsPort()){
            resultConnect = true;
          logFile->appendString("Порт "+portName+" открыт;");
       }
    }else{
        resultConnect = false;
        disconnectPort();
        emit sigError(thisPort->errorString());
        logFile->appendString("Ошибка открытия порта "+portName+";"+thisPort->errorString()+";");
    }
}

void SPort::disconnectPort()
{
    if(thisPort->isOpen()){
       thisPort->close();
       resultConnect = false;
       logFile->appendString("Порт "+portName+" закрыт;");

    }
}

QString SPort::getPortName() const
{
    return portName;
}

char SPort::crc(const char *buf, const int sz)
{
    int sum = 0;

    for(int i = 0; i < sz; i++)
        sum += buf[i];

    return sum;
}

void SPort::writePort(SPort* port, int size, int addr, int CMD, int type, bool *flag)
{
    prepareDataTX(port, addr, CMD);//Подготовка данных

    port->thisPort->write(TX,size);

    if(port->thisPort->waitForBytesWritten(WRITE_TIMEOUT)){
        if(flag){*flag = true;}
       logFile->appendString("Запись "+portName+";"+logFile->getHexFromBuf(TX,size));
    }else{
        if(flag){*flag = false;}
       emit sigError(thisPort->errorString());
       logFile->appendString("Ошибка записи в порт "+portName+"; "+thisPort->errorString()+"; ");
    }
}



bool SPort::getDataFromPort(SPort *port, int cmd, bool *flagOK, int type)
{
   if(port->thisPort->waitForReadyRead(READ_TIMEOUT)){
      RX.clear();
      qint64 cntByte = port->thisPort->bytesAvailable();
      RX = port->thisPort->readAll();
      logFile->appendString("Чтение "+ portName +";"+logFile->getHexFromBuf(RX.data(),cntByte));

      int sz = kvkSIZ0[cmd] >> 4;
      if(type == 4)
         sz = kvkSIZ01[cmd] >> 4;

      if(sz != cntByte){
         logFile->appendString("Ошибка чтения "+ portName +";"+" Получено бaйт "+QString::number(cntByte)+" вместо "+QString::number(sz));
         *flagOK = false;
      }else if((char)RX[sz-1] != crc(RX,sz-1)){
         logFile->appendString("Ошибка чтения "+ portName +";"+"Неправильная контрольная сумма!");
         *flagOK = false;
      }else if((quint8)RX[0] != (TX[0] | 0x80)){
         logFile->appendString("Ошибка чтения "+ portName +";"+"Пришел неправильный ответ!");
         *flagOK = false;
      }else{
          *flagOK = true;
      }
      return true;
   }else{
      return false;
   }
}

void SPort::saveSettingsModule(DataModules *data, quint8 adr, quint8 type, quint8 aver)
{
    bool trOK = false;
    int cmd = 14 | (2 << 4);
    TX[2] = adr;
    TX[3] = 0;
    TX[4] = type;
    TX[5] = aver;

    writePort(this, 7, data->getAddrDecMod().toInt(), cmd, data->getTypeModule(), &trOK);
    if(trOK){
        getDataFromPort(this, cmd, &trOK);
        if(!trOK){
            QMessageBox::information(nullptr, "Запись параметров модуля", "Ошибка при получении ответа!");
        }
    }else{
        QMessageBox::information(nullptr, "Запись параметров модуля", "Ошибка записи!");
    }
}

//Слот на получение и обработку ответа от ТМ и ОК///////////////////////
void SPort::detectModuleTmOk(SPort *port, int cmd, int addr, bool *flag)
{
    bool flagOK;
    int sz = kvkSIZ0[cmd] & 0x0F;
    writePort(port, sz, addr, cmd);
    getDataFromPort(port, cmd, &flagOK);
    *flag = flagOK;
}//====================================================================//

//Выдача команды в ТМ=============================================//
void SPort::writeStartTestTM(SPort *port, int cmd, int addr, int N)
{
    memset(TX,0,sizeof(TX));
    TX[3] = N;
    int sz = kvkSIZ0[cmd] & 0x0F;
    writePort(port, sz, addr, cmd);
}//===============================================================//


void SPort::handDataSearchModules(SPort *port, int line, int addr, int cmd)
{
    bool flagOK;

    if(!preparedDataRead(addr, cmd)){//Подготовка данных к приему
        return;
    }

    if(getDataFromPort(port, cmd, &flagOK)){
       if(!RX.isEmpty()){
          if(flagOK){
             RX[0] = line;//Номер внутренней линии//
             RX[1] = addr;//Не используется//
          }else{
             RX.fill('a');
          }
          emit dataReady(RX);//Выдаем сигнал о принятии данных из порта//
       }
    }
}


void SPort::detectModule(SPort *port, int cmd, int addr)
{
     bool flagOK;
     int sz = kvkSIZ0[cmd] & 0x0F;
     writePort(port, sz, addr, cmd);
     getDataFromPort(port, cmd, &flagOK);
     if(!flagOK){
         QMessageBox::information(NULL, "Сообщение", "При посылке команды произошла ошибка!", QMessageBox::Ok);
     }

}

//Выполнить измерения/////////////////////////////
void SPort::makeMeasur(int cntMeasur, DataModules *data, bool *ok, uint *izmer)
{   //a[i][j] = *(a + i*(max value of j) + j)

    uint *iAr = izmer;

    int cmd1 = 14 | (3 << 4);//Опрос АЦП 1-4
    int cmd2 = 12 | (3 << 4);//Опрос АЦП 5-8
    int type = data->getTypeModule();
    int adr = data->getAddrDecMod().toInt();

    if(type != 4){
       readAcp(cmd1, adr, ok);
       if(*ok){
          *(iAr + cntMeasur * 8 + 0) = (quint8)RX[2] + (quint8)(RX[3]<<8);
          *(iAr + cntMeasur * 8 + 1) = (quint8)RX[4] + (quint8)(RX[5]<<8);
       }

    }else{
        readAcp(cmd1, adr, ok, type);
        if(*ok){
           *(iAr + cntMeasur * 8 + 0) = (quint8)RX[2] + (quint8)(RX[3]<<8);
           *(iAr + cntMeasur * 8 + 1) = (quint8)RX[4] + (quint8)(RX[5]<<8);
           *(iAr + cntMeasur * 8 + 2) = (quint8)RX[6] + (quint8)(RX[7]<<8);
           *(iAr + cntMeasur * 8 + 3) = (quint8)RX[8] + (quint8)(RX[9]<<8);
        }
        readAcp(cmd2, adr, ok, type);
        if(*ok){
           *(iAr + cntMeasur * 8 + 4) = (quint8)RX[2] + (quint8)(RX[3]<<8);
           *(iAr + cntMeasur * 8 + 5) = (quint8)RX[4] + (quint8)(RX[5]<<8);
           *(iAr + cntMeasur * 8 + 6) = (quint8)RX[6] + (quint8)(RX[7]<<8);
           *(iAr + cntMeasur * 8 + 7) = (quint8)RX[8] + (quint8)(RX[9]<<8);
        }
    }
}


void SPort::readAcp(int cmd, int addr, bool *flagOk, int type)
{
    int sz = getSizeByte(type, cmd);

    writePort(this, sz, addr, cmd, type);
    getDataFromPort(this, cmd, flagOk);
}

int SPort::getSizeByte(int type, int command)
{
    int sz = kvkSIZ0[command] & 0x0F;
    if(type == 4)
       sz = kvkSIZ01[command] & 0x0F;
    return sz;
}


//Запись коэффициента калибровки///////////////////////////////////////////////////////////////////////////////////
void SPort::makeWriteCalFactor(int type, int ma20_40, int numCh, int adr, uint *izmer)
{
    bool trOK = false;
    int cmd = (numCh < 4)?(14 | (5 << 4)):(12 | (5 << 4));
    TX[2] = numCh+1;
    TX[3] = ma20_40;
    TX[4] = izmer[numCh] & 0xFF;
    TX[5] = izmer[numCh] >> 8;

    writePort(this, 7, adr, cmd, type, &trOK);
    if(trOK){
        getDataFromPort(this, cmd, &trOK);
        if(!trOK){
            QMessageBox::information(NULL, "Сообщение", "Ошибка чтения при записи калибровочного коэффициента!");
        }
    }else{
        QMessageBox::information(NULL, "Сообщение", "Ошибка записи калибровочного коэффициента!");
    }
}//================================================================================================================//


//Сброс коэффициентов==============================================================================================//
void SPort::ResetCalCoeff(int type, int addr)
{
    ResetCalCoeff(0, 0, 2000 & 0xFF, 2000 >> 8, 14 | (5 << 4), type, addr);
    QThread::usleep(100*1000);
    ResetCalCoeff(1, 0, 2000 & 0xFF, 2000 >> 8, 14 | (5 << 4), type, addr);

    if(type == 4){
        QThread::usleep(100*1000);
        ResetCalCoeff(0, 1, 10000 & 0xFF, 10000 >> 8, 14 | (5 << 4), type, addr);
        QThread::usleep(100*1000);
        ResetCalCoeff(1, 1, 10000 & 0xFF, 10000 >> 8, 14 | (5 << 4), type, addr);
        QThread::usleep(100*1000);
        ResetCalCoeff(0, 0, 2000 & 0xFF, 2000 >> 8, 12 | (5 << 4), type, addr);
        QThread::usleep(100*1000);
        ResetCalCoeff(1, 0, 2000 & 0xFF, 2000 >> 8, 12 | (5 << 4), type, addr);
        QThread::usleep(100*1000);
        ResetCalCoeff(0, 1, 10000 & 0xFF, 10000 >> 8, 12 | (5 << 4), type, addr);
        QThread::usleep(100*1000);
        ResetCalCoeff(1, 1, 10000 & 0xFF, 10000 >> 8, 12 | (5 << 4), type, addr);
    }
}


void SPort::ResetCalCoeff(int acp, int acpType, int p4, int p5, int cmd, int type, int adr)
{
    bool flOK = false;

    TX[2] = acp; // ACP
    TX[3] = acpType; // TYPE
    TX[4] = p4;
    TX[5] = p5;
    writePort(this, 7, adr, cmd, type, &flOK);
    if (flOK){
        getDataFromPort(this, cmd, &flOK);
        if(!flOK){
            QMessageBox::information(NULL, "Сообщение", "Ошибка чтения при записи сброса калибровочного коэффициента!");
        }
    }else{
        QMessageBox::information(NULL, "Сообщение", "Ошибка сброса коэффициента калибровки!");
    }
}//=================================================================================================================//

//Контрольные измерения=============================================================================================//
void SPort::controlMeasur(int func, int type, int addr, uint *izm, LogFile *log)
{
    bool flOK = false;
    int cmd1 = 0x00 | (func << 6);
    int cmd2 = 0x10 | (func << 6);
    int sz1 = getSizeByte(type,cmd1);
    int sz2 = getSizeByte(type,cmd2);

    writePort(this, sz1, addr, cmd1, type, &flOK);
    if (flOK){
        getDataFromPort(this, cmd1, &flOK, type);
        if(!flOK){
            QMessageBox::information(NULL, "Сообщение АЦП 1-4", "Ошибка чтения при записи контрольных измерений!");
        }else{
            izm[0] = ((quint8)RX[2] + ((quint8)RX[3]<<8))*2.0/1000.0;
            izm[1] = ((quint8)RX[4] + ((quint8)RX[5]<<8))*2.0/1000.0;
            if(type == 4){
               izm[2] = ((quint8)RX[6] + ((quint8)RX[7]<<8))*2.0/1000.0;
               izm[3] = ((quint8)RX[8] + ((quint8)RX[9]<<8))*2.0/1000.0;
            }
        }
    }else{
        QMessageBox::information(NULL, "Сообщение АЦП 1-4", "Ошибка записи контрольных измерений!");
    }

    if(type == 4){
        writePort(this, sz2, addr, cmd2, type, &flOK);
        if (flOK){
            getDataFromPort(this, cmd2, &flOK, type);
            if(!flOK){
                QMessageBox::information(NULL, "Сообщение АЦП 5-8", "Ошибка чтения при записи контрольных измерений!");
            }else{
                izm[4] = ((quint8)RX[2] + ((quint8)RX[3]<<8))*2.0/1000.0;
                izm[5] = ((quint8)RX[4] + ((quint8)RX[5]<<8))*2.0/1000.0;
                izm[6] = ((quint8)RX[6] + ((quint8)RX[7]<<8))*2.0/1000.0;
                izm[7] = ((quint8)RX[8] + ((quint8)RX[9]<<8))*2.0/1000.0;
            }
        }else{
            QMessageBox::information(NULL, "Сообщение АЦП 5-8", "Ошибка записи контрольных измерений!");
        }
    }

    if(log){
       QString str;
       if(type != 4){
          str = QString::number(izm[0])+"\t"+QString::number(izm[1])+"\t";
       }else{
          str = QString::number(izm[0])+"\t"+QString::number(izm[1])+"\t"+QString::number(izm[2])+"\t"+QString::number(izm[3])+"\t"+
                  QString::number(izm[4])+"\t"+QString::number(izm[5])+"\t"+QString::number(izm[6])+"\t"+QString::number(izm[7])+"\t";
       }
       log->addString(str);
    }
}//=================================================================================================================//

//Запись в АЦП модулей уставок////////////////////////////////////////////////////////////////////////////////////////
void SPort::writeSettingsAcp(DataModules *data, uint *low, uint *hi)
{
    if(data->getTypeModule() == 4){
       writeDataAcp(data->getAddrDecMod().toInt(), 11 | (8 << 4), data->getTypeModule(), low, hi);
       writeDataAcp(data->getAddrDecMod().toInt(), 11 | (9 << 4), data->getTypeModule(), low+1, hi+1);
       writeDataAcp(data->getAddrDecMod().toInt(), 11 | (10 << 4), data->getTypeModule(), low+2, hi+2);
       writeDataAcp(data->getAddrDecMod().toInt(), 11 | (11 << 4), data->getTypeModule(), low+3, hi+3);
       writeDataAcp(data->getAddrDecMod().toInt(), 11 | (12 << 4), data->getTypeModule(), low+4, hi+4);
       writeDataAcp(data->getAddrDecMod().toInt(), 11 | (13 << 4), data->getTypeModule(), low+5, hi+5);
       writeDataAcp(data->getAddrDecMod().toInt(), 11 | (14 << 4), data->getTypeModule(), low+6, hi+6);
       writeDataAcp(data->getAddrDecMod().toInt(), 11 | (15 << 4), data->getTypeModule(), low+7, hi+7);
    }else{
        bool flOK = false;
        TX[2] = (quint8)(*low & 0x00FF);
        TX[3] = (quint8)(*low >> 8);
        TX[4] = (quint8)(*hi & 0x00FF);
        TX[5] = (quint8)(*hi >> 8);
        TX[6] = (quint8)(*(low+1) & 0x00FF);
        TX[7] = (quint8)(*(low+1) >> 8);
        TX[8] = (quint8)(*(hi+1) & 0x00FF);
        TX[9] = (quint8)(*(hi+1) >> 8);

        writePort(this, 11, data->getAddrDecMod().toInt(), 14 | (11 << 4), data->getTypeModule(), &flOK);
        if (flOK){
            getDataFromPort(this, 14 | (11 << 4), &flOK);
            if(!flOK){
               QMessageBox::information(NULL, "Сообщение", "Ошибка чтения ответа при записи уставок!");
            }
        }else{
              QMessageBox::information(NULL, "Сообщение", "Ошибка записи уставок!");
        }
    }
}


void SPort::writeDataAcp(int adr, int cmd, int type, uint *low, uint *hi)
{
    bool flOK = false;
    TX[2] = (qint8)(*low & 0x00FF);
    TX[3] = (qint8)(*low >> 8);
    TX[4] = (qint8)(*hi & 0x00FF);
    TX[5] = (qint8)(*hi >> 8);

    writePort(this, 7, adr, cmd, type, &flOK);
    if (flOK){
        getDataFromPort(this, cmd, &flOK);
        if(!flOK){
           QMessageBox::information(nullptr, "Сообщение", "Ошибка чтения ответа при записи уставок!");
        }
    }else{
          QMessageBox::information(nullptr, "Сообщение", "Ошибка записи уставок!");
    }
}

/**
 * @brief SPort::writeDataAcpMBAC
 * @param numDac
 * @param specPar
 * @param adr
 * @param cmd
 * @param low
 * @param hi
 * @return
 */
bool SPort::writeDataAcpMBAC(quint8 numDac, quint8 specPar, quint8 adr, quint8 cmd, quint8 low, quint8 hi)
{
    const quint8 TR_SIZE = 7;
    quint8 TR[TR_SIZE]={0};

    TR[0] = adr;
    TR[1] = cmd;
    TR[2] = numDac ;
    TR[3] = specPar;
    TR[4] = low;
    TR[5] = hi;
    TR[6] = static_cast<quint8>(crc( reinterpret_cast<const char *>(TR), TR_SIZE));

    return writeSPort(TR, TR_SIZE);
}

/**
 * Запись калиброванного значения на ЦАПы 1-2 *
 * @brief SPort::writeDataAcp12MBAC
 * @param adr
 * @param cmd
 * @param low01
 * @param hi01
 * @param low02
 * @param hi02
 * @return
 */
bool SPort::writeDataAcp12MBAC(quint8 adr, quint8 cmd, quint8 low0, quint8 hi1, quint8 low2, quint8 hi3)
{
    const quint8 TR_SIZE = 7;
    quint8 TR[TR_SIZE]={0};

    TR[0] = adr;
    TR[1] = cmd;
    TR[2] = low0 ;
    TR[3] = hi1;
    TR[4] = low2;
    TR[5] = hi3;
    TR[6] = static_cast<quint8>(crc( reinterpret_cast<const char *>(TR), TR_SIZE));

    return writeSPort(TR, TR_SIZE);
}

/**
 * @brief SPort::writeSPort
 * @param tr
 * @param size
 * @return
 */
bool SPort::writeSPort(quint8 tr[], const quint8 size){
    bool flagOk = false;
    qint64 retByte = this->thisPort->write(reinterpret_cast<const char *>(tr),size);
    if(this->thisPort->waitForBytesWritten(WRITE_TIMEOUT) && (retByte > 0)){
       flagOk = true;
       logFile->appendString("Запись "+portName+";"+logFile->getHexFromBuf(reinterpret_cast<char *>(tr), size));
    }else{
       logFile->appendString("Ошибка записи в порт "+portName+"; "+thisPort->errorString()+"; ");
    }
    return flagOk;
}


/**
 * @brief SPort::getDataFromPortMBAC
 * @param adr
 * @param cmd
 * @param sz
 * @return
 */
bool SPort::getDataFromPortMBAC(quint8 adr, quint8 sz)
{
   bool flagOK = false;

   if(this->thisPort->waitForReadyRead(READ_TIMEOUT)){
      RX.clear();
      qint64 cntByte = this->thisPort->bytesAvailable();
      RX = this->thisPort->readAll();
      logFile->appendString("Чтение "+ portName +";"+logFile->getHexFromBuf(reinterpret_cast<char *>(RX.data()),static_cast<int>(cntByte)));

      if(sz != cntByte){
         logFile->appendString("Ошибка чтения "+ portName +";"+" Получено бaйт "+QString::number(cntByte)+" вместо "+QString::number(sz));
         flagOK = false;
      }else if(static_cast<char>(RX[sz-1]) != crc(RX,sz-1)){
         logFile->appendString("Ошибка чтения "+ portName +";"+"Неправильная контрольная сумма!");
         flagOK = false;
      }else if(static_cast<quint8>(RX[0]) != (adr | 0x80)){
          logFile->appendString("Ошибка чтения "+ portName +";"+"Пришел неправильный ответ!");
          flagOK = false;
      }else{
          flagOK = true;
      }
   }
   return flagOK;
}

quint16 convertByteToInt16(quint8 d0, quint8 d1){
    union byteToInt16{
      quint8 array[2];
      quint16 ret;
    };

    byteToInt16 bt ={{d0,d1}};
    return bt.ret;
}


//Константы скорости/////////////////////////////////////////////////////////////////////////////////////////////////
//Получить данные////////////////////////////////////////////////////////////////////////////////////////////////////
void SPort::ReadRateConst(DataModules *data, uint *low, uint *hi)
{
    if(data->getTypeModule() == 4){
        getDataAcp_1_8(data->getAddrDecMod().toInt(), 10|(0 << 4), data->getTypeModule(), low, hi);
        getDataAcp_1_8(data->getAddrDecMod().toInt(), 10|(1 << 4), data->getTypeModule(), low+1, hi+1);
        getDataAcp_1_8(data->getAddrDecMod().toInt(), 10|(2 << 4), data->getTypeModule(), low+2, hi+2);
        getDataAcp_1_8(data->getAddrDecMod().toInt(), 10|(3 << 4), data->getTypeModule(), low+3, hi+3);
        getDataAcp_1_8(data->getAddrDecMod().toInt(), 10|(4 << 4), data->getTypeModule(), low+4, hi+4);
        getDataAcp_1_8(data->getAddrDecMod().toInt(), 10|(5 << 4), data->getTypeModule(), low+5, hi+5);
        getDataAcp_1_8(data->getAddrDecMod().toInt(), 10|(6 << 4), data->getTypeModule(), low+6, hi+6);
        getDataAcp_1_8(data->getAddrDecMod().toInt(), 10|(7 << 4), data->getTypeModule(), low+7, hi+7);
    }else{
        getDataAcp_1_2(data->getAddrDecMod().toInt(), 14|(14 << 4), data->getTypeModule(), low, hi);
    }

}

//Запись констант скорости///////////////////////////////////////////////////////////////////////////////////////////
void SPort::WriteRateConst(DataModules *data, uint *low, uint *hi)
{
    if(data->getTypeModule() == 4){
       writeDataAcp(data->getAddrDecMod().toInt(), 10 | (8 << 4), data->getTypeModule(), low, hi);
       writeDataAcp(data->getAddrDecMod().toInt(), 10 | (9 << 4), data->getTypeModule(), low+1, hi+1);
       writeDataAcp(data->getAddrDecMod().toInt(), 10 | (10 << 4), data->getTypeModule(), low+2, hi+2);
       writeDataAcp(data->getAddrDecMod().toInt(), 10 | (11 << 4), data->getTypeModule(), low+3, hi+3);
       writeDataAcp(data->getAddrDecMod().toInt(), 10 | (12 << 4), data->getTypeModule(), low+4, hi+4);
       writeDataAcp(data->getAddrDecMod().toInt(), 10 | (13 << 4), data->getTypeModule(), low+5, hi+5);
       writeDataAcp(data->getAddrDecMod().toInt(), 10 | (14 << 4), data->getTypeModule(), low+6, hi+6);
       writeDataAcp(data->getAddrDecMod().toInt(), 10 | (15 << 4), data->getTypeModule(), low+7, hi+7);
    }else{
        bool flOK = false;
        TX[2] = (quint8)(*low & 0x00FF);
        TX[3] = (quint8)(*low >> 8);
        TX[4] = (quint8)(*hi & 0x00FF);
        TX[5] = (quint8)(*hi >> 8);
        TX[6] = (quint8)(*(low+1) & 0x00FF);
        TX[7] = (quint8)(*(low+1) >> 8);
        TX[8] = (quint8)(*(hi+1) & 0x00FF);
        TX[9] = (quint8)(*(hi+1) >> 8);

        writePort(this, 11, data->getAddrDecMod().toInt(), 14 | (15 << 4), data->getTypeModule(), &flOK);
        if (flOK){
            getDataFromPort(this, 14 | (11 << 4), &flOK);
            if(!flOK){
               QMessageBox::information(NULL, "Сообщение", "Ошибка чтения ответа при записи констант!");
            }
        }else{
              QMessageBox::information(NULL, "Сообщение", "Ошибка записи констант!");
        }
    }
}

//=================================================================================================================//


//Получение уставок АЦП от модуля/////////////////////////////////////////////////////////////////////////////////////
void SPort::readSettingsAcp(DataModules *data, uint *low, uint *hi)
{
    if(data->getTypeModule() == 4){
       getDataAcp_1_8(data->getAddrDecMod().toInt(), 11|(0 << 4), data->getTypeModule(), low, hi);
       getDataAcp_1_8(data->getAddrDecMod().toInt(), 11|(1 << 4), data->getTypeModule(), low+1, hi+1);
       getDataAcp_1_8(data->getAddrDecMod().toInt(), 11|(2 << 4), data->getTypeModule(), low+2, hi+2);
       getDataAcp_1_8(data->getAddrDecMod().toInt(), 11|(3 << 4), data->getTypeModule(), low+3, hi+3);
       getDataAcp_1_8(data->getAddrDecMod().toInt(), 11|(4 << 4), data->getTypeModule(), low+4, hi+4);
       getDataAcp_1_8(data->getAddrDecMod().toInt(), 11|(5 << 4), data->getTypeModule(), low+5, hi+5);
       getDataAcp_1_8(data->getAddrDecMod().toInt(), 11|(6 << 4), data->getTypeModule(), low+6, hi+6);
       getDataAcp_1_8(data->getAddrDecMod().toInt(), 11|(7 << 4), data->getTypeModule(), low+7, hi+7);
    }else{
        getDataAcp_1_2(data->getAddrDecMod().toInt(), 14|(10 << 4), data->getTypeModule(), low, hi);
    }
}

void SPort::getDataAcp_1_8(int adr, int cmd, int type, uint *low, uint *hi)
{
    bool flOK = false;

    writePort(this, 3, adr, cmd, type, &flOK);
    if (flOK){
        getDataFromPort(this, cmd, &flOK);
        if(flOK){
             *low = (quint8)RX[2] + ((quint8)RX[3] << 8);
             *hi  = (quint8)RX[4] + ((quint8)RX[5] << 8);
        }else{
           QMessageBox::information(NULL, "Сообщение", "Ошибка ответа от порта при чтения!");
        }
    }else{
          QMessageBox::information(NULL, "Сообщение", "Ошибка записи в порт при чтения!");
    }
}

void SPort::getDataAcp_1_2(int adr, int cmd, int type, uint *low, uint *hi)
{
    bool flOK = false;

    writePort(this, 3, adr, cmd, type, &flOK);
    if (flOK){
        getDataFromPort(this, cmd, &flOK);
        if(flOK){
             *low = (quint8)RX[2] + ((quint8)RX[3] << 8);
             *hi  = (quint8)RX[4] + ((quint8)RX[5] << 8);
            *(low+1) = (quint8)RX[6] + ((quint8)RX[7] << 8);
            *(hi+1)  = (quint8)RX[8] + ((quint8)RX[9] << 8);
        }else{
           QMessageBox::information(NULL, "Сообщение", "Ошибка чтения!");
        }
    }else{
          QMessageBox::information(NULL, "Сообщение", "Ошибка записи!");
    }


}
//==================================================================================================================//


//Подготовка данных к передаче.//////////////////////////////////////////////////////////////////////////////////////
void SPort::prepareDataTX(SPort *port, char addr, int cmd, int type)
{
    int sz = getSizeByte(type, cmd);

    if(sz){
       port->TX[0] = addr;
       port->TX[1] = cmd;
       port->TX[sz-1] = port->crc(port->TX, sz-1);
    }

}//================================================================================================================//




//Подготовка данных к приему.////////////////////////////////////////////////////////////////////////////////////////
bool SPort::preparedDataRead(char addr, int cmd)
{
    int sz = kvkSIZ0[cmd] >> 4;
    if ((sz) && ((addr != 0x31) || (cmd == 0x1E) || (cmd == 0x2E)))
        return true;
    return false;
}//================================================================================================================//

QByteArray SPort::readPortAll(SPort* port)
{
    QByteArray ar = port->thisPort->readAll();
    return ar;
}

void SPort::readPort(SPort* port, qint64 size)
{
    port->thisPort->read(RX.data(), size);
}

bool SPort::writeSettingsPort()
{
    bool result = false;

    //QSerialPort::BaudRate bod = (QSerialPort::BaudRate)((config*)parent())->getCmbBod()->itemText(XmlDocument::cfgMap.value("BaudRate")).toInt();
    QSerialPort::BaudRate bod = (QSerialPort::BaudRate)XmlDocument::cfgMap.value("BitSec");
    if(thisPort->setBaudRate(bod) && thisPort->setDataBits((QSerialPort::DataBits)XmlDocument::cfgMap.value("ByteSize"))
                                  && thisPort->setParity((QSerialPort::Parity)XmlDocument::cfgMap.value("Parity"))
                                  && thisPort->setStopBits((QSerialPort::StopBits)XmlDocument::cfgMap.value("StopBit"))){
       result = true;
       //qDebug()<<"Порт "<<portName<<" настроен";
    }

    return result;
}

