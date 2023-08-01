#ifndef DATAMODULES_H
#define DATAMODULES_H
#include <QString>

// Описание модуля//////////////////////////////////////////////////////////////////////////
class DataModules
{
public:
    DataModules();
    DataModules(QString numline, QString numinternal, QString addrdecmod, QString addrhexmod,
                QString addrplo, QString typemod, QString statusinver, QString setaver,
                QString versionmod, QString idmod, QString reservmod);
    ~DataModules();

    QString getNumLine() const;
    void setNumLine(const QString &value);

    QString getNumInternal() const;
    void setNumInternal(const QString &value);

    QString getAddrDecMod() const;
    void setAddrDecMod(const QString &value);

    QString getAddrHexMod() const;
    void setAddrHexMod(const QString &value);

    QString getAddrPLO() const;
    void setAddrPLO(const QString &value);

    QString getTypeMod() const;
    void setTypeMod(const QString &value);

    QString getStatusInver() const;
    void setStatusInver(const QString &value);

    QString getSetAver() const;
    void setSetAver(const QString &value);

    QString getVersionMod() const;
    void setVersionMod(const QString &value);

    QString getIdMod() const;
    void setIdMod(const QString &value);

    QString getReservMod() const;
    void setReservMod(const QString &value);
    int getCntColumns() const;

    uint getColorModuleRow() const;
    void setColorModuleRow(uint value);

    int getTypeModule() const;
    void setTypeModule(int value);

    quint16 getAdcAddr() const;
    void setAdcAddr(const quint16 &value);
    bool isData();

private:
    int cntColumns;
    QString numLine;//Номер линии
    QString numInternal;//Внутренний номер
    QString addrDecMod;//Адрес модуля десятичный
    QString addrHexMod;//Адрес модуля шестнадцатиричный
    QString addrPLO;//Адрес программы логической обработки
    QString typeMod;//Тип модуля
    QString statusInver;//Инверсия
    QString setAver;//Усреднение
    QString versionMod;//Версия модуля
    QString idMod;//ИД модуля
    QString reservMod;//Резервные модули
    int typeModule;//Номер типа модуля
    uint colorModuleRow;//Цвет строки в таблице
    quint16 adcAddr;//Первый аналоговый параметр//

};

#endif // DATAMODULES_H
