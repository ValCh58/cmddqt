#include "datamodules.h"

/**
 * @brief DataModules::DataModules
 */
DataModules::DataModules(){

    cntColumns=0;
    colorModuleRow=0;
    typeModule=0;
    adcAddr=0;
}

/**
 * @brief DataModules::DataModules
 * @param numline
 * @param numinternal
 * @param addrdecmod
 * @param addrhexmod
 * @param addrplo
 * @param typemod
 * @param statusinver
 * @param setaver
 * @param versionmod
 * @param idmod
 * @param reservmod
 */
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

/**
 * @brief DataModules::~DataModules
 */
DataModules::~DataModules()
{

}

QString DataModules::getReservMod() const
{
    return reservMod;
}

/**
 * @brief DataModules::setReservMod
 * @param value
 */
void DataModules::setReservMod(const QString &value)
{
    reservMod = value;
}
/**
 * @brief DataModules::getCntColumns
 * @return
 */
int DataModules::getCntColumns() const
{
    return cntColumns;
}
/**
 * @brief DataModules::getColorModuleRow
 * @return
 */
uint DataModules::getColorModuleRow() const
{
    return colorModuleRow;
}

/**
 * @brief DataModules::setColorModuleRow
 * @param value
 */
void DataModules::setColorModuleRow(uint value)
{
    colorModuleRow = value;
}

/**
 * @brief DataModules::getTypeModule
 * @return
 */
int DataModules::getTypeModule() const
{
    return typeModule;
}

/**
 * @brief DataModules::setTypeModule
 * @param value
 */
void DataModules::setTypeModule(int value)
{
    typeModule = value;
}

/**
 * @brief DataModules::getAdcAddr
 * @return
 */
quint16 DataModules::getAdcAddr() const
{
    return adcAddr;
}

/**
 * @brief DataModules::setAdcAddr
 * @param value
 */
void DataModules::setAdcAddr(const quint16 &value)
{
    adcAddr = value;
}

/**
 * @brief DataModules::isData
 * @return
 */
bool DataModules::isData()
{
    return getNumLine().toInt() > 0;
}

/**
 * @brief DataModules::getVersionMod
 * @return
 */
QString DataModules::getVersionMod() const
{
    return versionMod;
}

/**
 * @brief DataModules::setVersionMod
 * @param value
 */
void DataModules::setVersionMod(const QString &value)
{
    versionMod = value;
}

/**
 * @brief DataModules::getIdMod
 * @return
 */
QString DataModules::getIdMod() const
{
    return idMod;
}

/**
 * @brief DataModules::setIdMod
 * @param value
 */
void DataModules::setIdMod(const QString &value)
{
    idMod = value;
}

/**
 * @brief DataModules::getSetAver
 * @return
 */
QString DataModules::getSetAver() const
{
    return setAver;
}

/**
 * @brief DataModules::setSetAver
 * @param value
 */
void DataModules::setSetAver(const QString &value)
{
    setAver = value;
}

/**
 * @brief DataModules::getStatusInver
 * @return
 */
QString DataModules::getStatusInver() const
{
    return statusInver;
}

/**
 * @brief DataModules::setStatusInver
 * @param value
 */
void DataModules::setStatusInver(const QString &value)
{
    statusInver = value;
}

/**
 * @brief DataModules::getTypeMod
 * @return
 */
QString DataModules::getTypeMod() const
{
    return typeMod;
}

/**
 * @brief DataModules::setTypeMod
 * @param value
 */
void DataModules::setTypeMod(const QString &value)
{
    typeMod = value;
}

/**
 * @brief DataModules::getAddrPLO
 * @return
 */
QString DataModules::getAddrPLO() const
{
    return addrPLO;
}

/**
 * @brief DataModules::setAddrPLO
 * @param value
 */
void DataModules::setAddrPLO(const QString &value)
{
    addrPLO = value;
}

/**
 * @brief DataModules::getAddrHexMod
 * @return
 */
QString DataModules::getAddrHexMod() const
{
    return addrHexMod;
}

/**
 * @brief DataModules::setAddrHexMod
 * @param value
 */
void DataModules::setAddrHexMod(const QString &value)
{
    addrHexMod = value;
}

/**
 * @brief DataModules::getAddrDecMod
 * @return
 */
QString DataModules::getAddrDecMod() const
{
    return addrDecMod;
}

/**
 * @brief DataModules::setAddrDecMod
 * @param value
 */
void DataModules::setAddrDecMod(const QString &value)
{
    addrDecMod = value;
}

/**
 * @brief DataModules::getNumInternal
 * @return
 */
QString DataModules::getNumInternal() const
{
    return numInternal;
}

/**
 * @brief DataModules::setNumInternal
 * @param value
 */
void DataModules::setNumInternal(const QString &value)
{
    numInternal = value;
}

/**
 * @brief DataModules::getNumLine
 * @return
 */
QString DataModules::getNumLine() const
{
    return numLine;
}

/**
 * @brief DataModules::setNumLine
 * @param value
 */
void DataModules::setNumLine(const QString &value)
{
    numLine = value;
}


