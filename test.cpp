#include "test.h"
#include "ccmbb.h"


Test::Test(TiXmlElement *e)
{
    int i;
    disableStart = 0;
    disableStop = 0;
    discr = 0;
    memset(Result, 0, sizeof(Result));
    TiXmlElement* c;
    TiXmlElement* n;
    TiXmlText* t;
    //Прочитаем атрибуты теста//
    Assert_x(e->Attribute("Nex",&Nex), "Сообщение", "CFG: Указан тест без собственного номера(Nex)");
    setDisableState(Nex);
    Assert_x(e->Attribute("N",&N), "Сообщение", "CFG: Указан тест без внутреннего номера(N)");
    Assert_x(e->Attribute("ImageIndex",&ImageIndex), "Сообщение", "CFG: Указан тест безномера иконки(ImageIndex)");
    Assert_x(e->Attribute("Buttons",&Buttons), "Сообщение", "CFG: Указан тест без кнопок(Buttons)");
    Assert_x(e->Attribute("Mbb",&MBB), "Сообщение", "CFG: Указан тест без модулей(Mbb)");
    //читаем текстовые поля теста
    c = e->FirstChildElement("Caption");
    t = (TiXmlText*)c->FirstChild();
    Caption = get1251String(makestr((char*)t->Value()));
    c = e->FirstChildElement("Abstract");
    t = (TiXmlText*)c->FirstChild();
    Abstract = get1251String((char*)makestr((char*)t->Value()));
    c = e->FirstChildElement("Message");
    t = (TiXmlText*)c->FirstChild();
    Message = get1251String((char*)makestr((char*)t->Value()));
    c = e->FirstChildElement("Script");
    t = (TiXmlText*)c->FirstChild();
    Script = get1251String((char*)makestr((char*)t->Value()));
    //читаем сообщения теста
    msgs = new Msgs();
    c = e->FirstChildElement("Messages");
    Assert_x(e, "Сообщение", "CFG: Отсутствует список сообщений у одного из тестов(Messages)");
    for( n = c->FirstChildElement("Message"); n; n = n->NextSiblingElement() )
    {
        Assert_x(n->Attribute("Code",&i), "Сообщение", "CFG: Указанное сообщение без кода(Code)");
        t = (TiXmlText*)n->FirstChild();
        //Добавим с список сообщений
        msgs->insert(i,get1251String((char*)t->Value()));
    }
}

QString Test::get1251String(char *buf)
{
    //Изменим кодировку===============//
    QTextStream ts(new QByteArray(buf));
    ts.setCodec("Windows-1251");
    QString s=ts.readAll();
    return s;
}

void Test::setDisableState(int nex)
{
    switch(nex){

    case 31: //Калибровка узла измерения тока №1, включим соседа//
    case 32: //Калибровка узла измерения тока №2, включим соседа//
    case 40: //Проверка узла измерения тока №1, включим соседа//
    case 41: //Проверка узла измерения тока №2, включим соседа//
    case 42: //Проверка узла измерения частоты №1, включим соседа//
    case 43: //Проверка узла измерения частоты №2, включим соседа//
        disableStart = BTNSTOP | BTNREADY;
        disableStop  = BTNSTART | BTNCLEAR;
        break;

    case 36: //НВС, сбросим все ошибки//Проверка узла ввода дискретных сигналов OK НBС
        disableStart = BTNSTOP | BTNREADY;
        disableStop  = BTNSTART | BTNCLEAR;
        break;

    case 34: //РДС, сбросим все ошибки//
    case 33: //КДС, сбросим все ошибки//
    case 35: //ННС, сбросить все ошибки//Проверка узла ввода дискретных сигналов OK ННС
    case 1: //Проверка таймеров//
    case 2: //Проверка сигналов KNO, AWO//
    case 5: //Проверка UART1//
    case 6: //Проверка UART0//
        disableStart = BTNSTOP;
        disableStop  = BTNSTART | BTNCLEAR;
        break;

    case 3:
    case 4: //Проверка светодиодов//
        disableStart = BTNYES | BTNNO;
        disableStop  = BTNSTART | BTNCLEAR;
        break;
    }
}

QString Test::getMessage() const
{
    return Message;
}

void Test::setMessage(const QString &value)
{
    Message = value;
}

QString Test::getAbstract() const
{
    return Abstract;
}

void Test::setAbstract(const QString &value)
{
    Abstract = value;
}

QString Test::getCaption() const
{
    return Caption;
}

void Test::setCaption(const QString &value)
{
    Caption = value;
}

int Test::getMBB() const
{
    return MBB;
}

void Test::setMBB(int value)
{
    MBB = value;
}

int Test::getN() const
{
    return N;
}

void Test::setN(int value)
{
    N = value;
}

int Test::getNex() const
{
    return Nex;
}

void Test::setNex(int value)
{
    Nex = value;
}

int Test::getOrder() const
{
    return Order;
}

void Test::setOrder(int value)
{
    Order = value;
}



Test::~Test()
{

}

char *Test::makestr(char *s)
{
    char* d = new char[strlen(s)+1];
    strcpy(d, s);
    return d;

}



