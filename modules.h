#ifndef MODULES_H
#define MODULES_H

#include <QString>

class Modules
{

public:
    Modules();
    ~Modules();

    int getType() const;
    void setType(int value);

    uint getColor() const;
    void setColor(uint value);

    QString getName() const;
    void setName(const QString &value);

private:
    int type;
    uint color;
    QString name;
};

class AdcPloAddr{

private:
    QString ploAddr;
    quint16 adcAddr;


public:
    QString getPloAddr() const;
    void setPloAddr(const QString &value);
    quint16 getAdcAddr() const;
    void setAdcAddr(const quint16 &value);
};

class Adcs{

public:
    QString getName() const;
    void setName(const QString &value);

    QString getShortName() const;
    void setShortName(const QString &value);

private:
    QString name;
    QString shortName;

};

#endif // MODULES_H
