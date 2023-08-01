#ifndef TEST_H
#define TEST_H

#include <QVector>
#include <QMap>
#include <QString>
#include "tinyxml\tinyxml.h"

#define BTNPREV  0x01
#define BTNNEXT  0x02
#define BTNSTART 0x04
#define BTNSTOP  0x08
#define BTNREADY 0x10
#define BTNYES   0x20
#define BTNNO    0x40
#define BTNCLEAR 0x80

typedef QMap<quint8, QString> Msgs;

class Test
{
public:
    Test(TiXmlElement* e);
    ~Test();
private:
    char *makestr(char* s);
    QString get1251String(char *buf);


public:
    //Порядок добавления тестов
    int Order;
    //Собственный номер теста в CMBB
    int Nex;
    //Внутренний номер теста в СMBB
    int N;
    //Номер картинки
    int ImageIndex;
    //Маска кнопок
    int Buttons;
    //Маска разрешенных модулей
    int MBB;
    //Заголовок теста
    QString Caption;
    //Описание теста
    QString Abstract;
    //Сообщение после старта теста
    QString Message;
    //Скрипт, который выполняется
    QString Script;
    //Результаты тестов
    uint  Result[32];
    //Отклонения АЦП
    float  acp_otkl[2];
    //Ошибки дискретных сигналов
    ulong  discr;
    //Сообщения во время тестов
    Msgs* msgs;//QMap!!!
    //Блокировка кнопок//
    void setDisableState(int nex);
    quint8 disableStop;
    quint8 disableStart;
    int getOrder() const;
    void setOrder(int value);
    int getNex() const;
    void setNex(int value);
    int getN() const;
    void setN(int value);
    int getMBB() const;
    void setMBB(int value);
    QString getCaption() const;
    void setCaption(const QString &value);
    QString getAbstract() const;
    void setAbstract(const QString &value);
    QString getMessage() const;
    void setMessage(const QString &value);
};

typedef QVector< Test* > Tests;


#endif // TEST_H
