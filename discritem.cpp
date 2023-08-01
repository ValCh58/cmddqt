#include "discritem.h"
#include "discretrds.h"

DiscrItem::DiscrItem(quint32 id, QString hexAdrPlo, QWidget *parent) : QWidget(parent)
{
    discrRds = (discretRDS*)parent;
    itemId = id;
    adrPlo = hexAdrPlo;
    comWord = 0;
    vBoxItem = new QVBoxLayout();
    ind1 = QPixmap(":/cmbbImages/ind1.png");
    ind2 = QPixmap(":/cmbbImages/ind2.png");
    indRead = new QLabel();
    indRead->setFixedSize(24, 24);
    indRead->setPixmap(ind1);
    indRead->setAlignment(Qt::AlignCenter);
    pButton = new QPushButton(QString::number(id+1));
    pButton->setFixedSize(24, 24);
    pButton->setCursor(Qt::PointingHandCursor);
    lblAdr = new QLabel(adrPlo);
    lblAdr->setFixedSize(24, 20);
    lblAdr->setAlignment(Qt::AlignCenter);
    vBoxItem->addWidget(indRead);
    vBoxItem->addWidget(pButton);
    vBoxItem->addWidget(lblAdr);
    stateBtn = false;//Кнопка красного цвета - выключенo//
    stateInd = false;//Индикация чтения красного цвета, соответствует состоянию кнопки//
    pButton->setAutoFillBackground(true);
    setColorItem(stateBtn);
    connect(pButton, SIGNAL(clicked()), this, SLOT(ctlStateButton()));
}

DiscrItem::~DiscrItem()
{
  delete indRead;
  delete lblAdr;
  delete pButton;
  delete vBoxItem;
}
bool DiscrItem::getStateBtn() const
{
    return stateBtn;
}

void DiscrItem::setStateBtn(bool value)
{
    stateBtn = value;
}
bool DiscrItem::getStateInd() const
{
    return stateInd;
}

void DiscrItem::setStateInd(bool value)
{
    stateInd = value;
}


//Управление цветом кнопок//
void DiscrItem::setColorItem(bool clr)
{
    if(clr)
        pButton->setStyleSheet(QString("background-color: rgb(0,170,0);color: rgb(255,255,0)"));
    else
        pButton->setStyleSheet(QString("background-color: rgb(200,0,0);color: rgb(0,255,255)"));
}

//Формирование командного слова для посылки в РДС//
void DiscrItem::ctlStateButton()
{
    quint32 tmp=1;
    tmp <<= itemId;

    if(!stateBtn){
       discrRds->setComWord(discrRds->getComWord()|tmp);//Включено//
       stateBtn = true;
    }else if(stateBtn){
        discrRds->setComWord(discrRds->getComWord()&(tmp^0XFFFFFFFF));//Выключено//
        stateBtn = false;
    }

    setColorItem(stateBtn);
}

void DiscrItem::setColorInd(bool clr)
{
    if(clr)
        indRead->setPixmap(ind2);
    else
        indRead->setPixmap(ind1);
}


