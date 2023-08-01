#include "datamodules.h"

DataModules::DataModules(){

    cntColumns=0;
    colorModuleRow=0;
    typeModule=0;
    adcAddr=0;
}

DataModules::DataModules(QString numline, QString numinternal, QString addrdecmod, QString addrhexmod,
                    QString addrplo, QString typemod, QString statusinver, QString setaver, QString versionmod, QString idmod, QString reservmod)
{
    numLine     = numline;
    numInternal = numinternal;
    addrDecMod  = addrdecmod;
    addrHexMod  = addrhexmod;
    addrPLO     = addrplo;
    typeMod     = typemod;
    statusInver = statusinver;
    setAver     = setaver;
    versionMod  = versionmod;
    idMod       = idmod;
    reservMod   = reservmod;

}

DataModules::~DataModules()
{

}

QString DataModules::getReservMod() const
{
    return reservMod;
}

void DataModules::setReservMod(const QString &value)
{
    reservMod = value;
}
int DataModules::getCntColumns() const
{
    return cntColumns;
}
uint DataModules::getColorModuleRow() const
{
    return colorModuleRow;
}

void DataModules::setColorModuleRow(uint value)
{
    colorModuleRow = value;
}
int DataModules::getTypeModule() const
{
    return typeModule;
}

void DataModules::setTypeModule(int value)
{
    typeModule = value;
}

quint16 DataModules::getAdcAddr() const
{
    return adcAddr;
}

void DataModules::setAdcAddr(const quint16 &value)
{
    adcAddr = value;
}

bool DataModules::isData()
{
    return getNumLine().toInt() > 0;
}

QString DataModules::getVersionMod() const
{
    return versionMod;
}

void DataModules::setVersionMod(const QString &value)
{
    versionMod = value;
}


QString DataModules::getIdMod() const
{
    return idMod;
}

void DataModules::setIdMod(const QString &value)
{
    idMod = value;
}

QString DataModules::getSetAver() const
{
    return setAver;
}

void DataModules::setSetAver(const QString &value)
{
    setAver = value;
}

QString DataModules::getStatusInver() const
{
    return statusInver;
}

void DataModules::setStatusInver(const QString &value)
{
    statusInver = value;
}

QString DataModules::getTypeMod() const
{
    return typeMod;
}

void DataModules::setTypeMod(const QString &value)
{
    typeMod = value;
}

QString DataModules::getAddrPLO() const
{
    return addrPLO;
}

void DataModules::setAddrPLO(const QString &value)
{
    addrPLO = value;
}

QString DataModules::getAddrHexMod() const
{
    return addrHexMod;
}

void DataModules::setAddrHexMod(const QString &value)
{
    addrHexMod = value;
}

QString DataModules::getAddrDecMod() const
{
    return addrDecMod;
}

void DataModules::setAddrDecMod(const QString &value)
{
    addrDecMod = value;
}

QString DataModules::getNumInternal() const
{
    return numInternal;
}

void DataModules::setNumInternal(const QString &value)
{
    numInternal = value;
}

QString DataModules::getNumLine() const
{
    return numLine;
}

void DataModules::setNumLine(const QString &value)
{
    numLine = value;
}


