#include "mydataport.h"
#include "xmldocument.h"

MyDataPort::MyDataPort(QString portname, QObject *parent)
    : QObject(parent)
{
    WRITE_TIMEOUT = 100;
    READ_TIMEOUT  = 100;
    portName = portname;
    thisPort = new QSerialPort(this);
    thisPort->setPortName(portName);
    QDir d;
    QString path = d.currentPath()+QDir::separator()+"log"+QDir::separator()+"port"+portName.right(4)+".log";
    logFile = new LogFile(path);

}

MyDataPort::~MyDataPort(){
    disconnectPort();
    delete thisPort;
    delete logFile;
}

/**
 * Запись конфигурации порта *
 * @brief MyDataPort::writeSettingsPort
 * @return bool
 */
bool MyDataPort::writeSettingsPort()
{
    bool result = false;
    QSerialPort::BaudRate bod = static_cast<QSerialPort::BaudRate> (XmlDocument::cfgMap.value("BitSec"));
    if(thisPort->setBaudRate(bod) && thisPort->setDataBits(static_cast<QSerialPort::DataBits> (XmlDocument::cfgMap.value("ByteSize")))
                                  && thisPort->setParity(static_cast<QSerialPort::Parity> (XmlDocument::cfgMap.value("Parity")))
                                  && thisPort->setStopBits(static_cast<QSerialPort::StopBits> (XmlDocument::cfgMap.value("StopBit")))){
       result = true;
    }
    return result;
}

/**
 * Получить данные из порта *
 * @brief MyDataPort::getDataFromPort
 * @param port
 * @param cmd
 * @param flagOK
 * @param type
 * @return
 */
bool MyDataPort::getDataFromPort(MyDataPort *port, int cmd, bool *flagOK, int type)
{
    if(port->thisPort->waitForReadyRead(READ_TIMEOUT)){
       RX.clear();
       qint64 cntByte = port->thisPort->bytesAvailable();
       RX = port->thisPort->readAll();
       logFile->appendString("Чтение "+ portName +";"+logFile->getHexFromBuf(RX.data(),cntByte));//implicit conversion loses integer precision: 'qint64' (aka 'long long') to 'int'

       int sz = kvkSIZ0[cmd] >> 4;
       if(type == 4)
          sz = kvkSIZ01[cmd] >> 4;

       if(sz != cntByte){
          logFile->appendString("Ошибка чтения "+ portName +";"+" Получено бaйт "+QString::number(cntByte)+" вместо "+QString::number(sz));
          *flagOK = false;
       }else if(static_cast<quint8>(RX[sz-1]) != crc(reinterpret_cast<const quint8 *>(RX.constData()),sz-1)){
          logFile->appendString("Ошибка чтения "+ portName +";"+"Неправильная контрольная сумма!");
          *flagOK = false;
       }else if(static_cast<quint8>(RX[0]) != (TX[0] | 0x80)){
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

/**
 * @brief MyDataPort::prepareDataTX
 * @param port
 * @param addr
 * @param cmd
 * @param type
 */
void MyDataPort::prepareDataTX(MyDataPort *port, quint8 addr, quint8 cmd, int type)
{
    int sz = getSizeByte(type, cmd);
    if(sz){
       port->TX[0] = addr;
       port->TX[1] = cmd;
       port->TX[sz-1] = port->crc(port->TX, sz-1);
    }
}

/**
 * @brief MyDataPort::preparedDataRead
 * @param addr
 * @param cmd
 * @return
 */
bool MyDataPort::preparedDataRead(char addr, int cmd)
{
    int sz = kvkSIZ0[cmd] >> 4;
    if ((sz) && ((addr != 0x31) || (cmd == 0x1E) || (cmd == 0x2E)))
        return true;
    return false;
}

/**
 * @brief MyDataPort::writeDataAcp
 * @param adrMod
 * @param cmd
 * @param numDac
 * @param srvDat
 * @param lowByte
 * @param hiByte
 */
void MyDataPort::writeDataAcp(quint8 adrMod, quint8 cmd, quint8 numDac, quint8 srvDat, quint8 *lowByte, quint8 *hiByte)
{
    bool flOK = false;
//    TX[2] = (*lowByte & 0x00FF);
//    TX[3] = (static_cast<quint8>(*lowByte) >> 8);
//    TX[4] = (*hiByte & 0x00FF);
//    TX[5] = (static_cast<quint8>(*hiByte) >> 8);
    //  TX[0] =

    writePort(this, 7, adrMod, cmd, &flOK);
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
 * @brief MyDataPort::getSizeByte
 * @param type
 * @param command
 * @return
 */
int MyDataPort::getSizeByte(int type, int command)
{
    int sz = kvkSIZ0[command] & 0x0F;
    if(type == 4)
       sz = kvkSIZ01[command] & 0x0F;
    return sz;
}

/**
 * Запись в порт *
 * @brief MyDataPort::writePort
 * @param port
 * @param size
 * @param addr
 * @param CMD
 * @param type
 * @param flag
 */
void MyDataPort::writePort(MyDataPort *port, int size, int addr, int CMD, bool *flag)
{
    prepareDataTX(port, static_cast<quint8>(addr), static_cast<quint8>(CMD));//Подготовка данных

    port->thisPort->write(reinterpret_cast<const char*>(TX),size);

    if(port->thisPort->waitForBytesWritten(WRITE_TIMEOUT)){
        if(flag){*flag = true;}
       logFile->appendString("Запись "+portName+";"+logFile->getHexFromBuf(reinterpret_cast<char*>(TX),size));
    }else{
        if(flag){*flag = false;}
       emit sigError(thisPort->errorString());
       logFile->appendString("Ошибка записи в порт "+portName+"; "+thisPort->errorString()+"; ");
    }
}

/**
 * Установка соединения с портом *
 * @brief MyDataPort::connectPort
 */
void MyDataPort::connectPort()
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

/**
 * Закрытие порта *
 * @brief MyDataPort::disconnectPort
 */
void MyDataPort::disconnectPort()
{
    if(thisPort->isOpen()){
       thisPort->close();
       resultConnect = false;
       logFile->appendString("Порт "+portName+" закрыт;");

    }
}

/**
 * Поlучить имя порта *
 * @brief SPort::getPortName
 * @return QString
 */
QString MyDataPort::getPortName() const
{
    return portName;
}

/**
 * Получить указатель на порт *
 * @brief MyDataPort::getThisPort
 * @return QSerialPort
 */
QSerialPort *MyDataPort::getThisPort() const
{
    return thisPort;
}

/**
 * Подсчет контрольной суммы *
 * @brief MyDataPort::crc
 * @param buf массив с данными
 * @param sz
 * @return
 */
quint8 MyDataPort::crc(const quint8 *buf, const int sz)
{
    quint8 sum = 0;
    for(int i = 0; i < sz; i++)
        sum += buf[i];
    return sum;
}
