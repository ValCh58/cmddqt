#include <QTextStream>
#include <QVector>
#include "modules.h"

//Modules==========
/**
 * @brief Modules::Modules
 */
Modules::Modules()
{

}

/**
 * @brief Modules::~Modules
 */
Modules::~Modules()
{

}

/**
 * @brief Modules::getType
 * @return
 */
int Modules::getType() const
{
    return type;
}

/**
 * @brief Modules::setType
 * @param value
 */
void Modules::setType(int value)
{
    type = value;
}

/**
 * @brief Modules::getColor
 * @return
 */
uint Modules::getColor() const
{
    return color;
}

/**
 * @brief Modules::setColor
 * @param value
 */
void Modules::setColor(uint value)
{
    color = value;
}

/**
 * @brief Modules::getName
 * @return
 */
QString Modules::getName() const
{
    return name;
}

/**
 * @brief Modules::setName
 * @param value
 */
void Modules::setName(const QString &value)
{
    name = value;
}

//Список модулей и ПЛО///////////////////////////////////////
//AdcPloAddr===============================================//
/**
 * @brief AdcPloAddr::getAdcAddr
 * @return
 */
quint16 AdcPloAddr::getAdcAddr() const
{
    return adcAddr;
}

/**
 * @brief AdcPloAddr::setAdcAddr
 * @param value
 */
void AdcPloAddr::setAdcAddr(const quint16 &value)
{
    adcAddr = value;
}

/**
 * @brief AdcPloAddr::getPloAddr
 * @return
 */
QString AdcPloAddr::getPloAddr() const
{
    return ploAddr;
}

/**
 * @brief AdcPloAddr::setPloAddr
 * @param value
 */
void AdcPloAddr::setPloAddr(const QString &value)
{
    ploAddr = value;
}

//Аналоговые параметры///////////////////////////////////////
//Adcs=====================================================//
/**
 * @brief Adcs::getName
 * @return
 */
QString Adcs::getName() const
{
    return name;
}

/**
 * @brief Adcs::setName
 * @param value
 */
void Adcs::setName(const QString &value)
{
    name = value;
}

/**
 * @brief Adcs::getShortName
 * @return
 */
QString Adcs::getShortName() const
{

    //QByteArray ba = shortName.toUtf8();
    //QTextStream sstr(&ba);
    //sstr.setAutoDetectUnicode(false);
    //QTextCodec *c = sstr.codec();
    //sstr.setCodec("UTF-8");
    //QString decodedStr = sstr.readAll();

    return shortName;
}

/**
 * @brief Adcs::setShortName
 * @param value
 */
void Adcs::setShortName(const QString &value)
{
    shortName = value;
}

