#include <QTextStream>
#include <QVector>
#include "modules.h"

//Modules==================================================//
Modules::Modules()
{

}

Modules::~Modules()
{

}
int Modules::getType() const
{
    return type;
}

void Modules::setType(int value)
{
    type = value;
}
uint Modules::getColor() const
{
    return color;
}

void Modules::setColor(uint value)
{
    color = value;
}
QString Modules::getName() const
{
    return name;
}

void Modules::setName(const QString &value)
{
    name = value;
}

//Список модулей и ПЛО///////////////////////////////////////
//AdcPloAddr===============================================//
quint16 AdcPloAddr::getAdcAddr() const
{
    return adcAddr;
}

void AdcPloAddr::setAdcAddr(const quint16 &value)
{
    adcAddr = value;
}
QString AdcPloAddr::getPloAddr() const
{
    return ploAddr;
}

void AdcPloAddr::setPloAddr(const QString &value)
{
    ploAddr = value;
}

//Аналоговые параметры///////////////////////////////////////
//Adcs=====================================================//
QString Adcs::getName() const
{
    return name;
}

void Adcs::setName(const QString &value)
{
    name = value;
}
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

void Adcs::setShortName(const QString &value)
{
    shortName = value;
}

