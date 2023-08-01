#include <QDebug>

#include "discretrds.h"
#include "datamodules.h"
#include "testrds.h"

discretRDS::discretRDS(int type, DataModules mod, QList<DataModules> list, QWidget *parent) : QWidget(parent)
{

    module = mod;
    nncList = new QList<DataModules>(list);
    lineNNC = 1;
    QGroupBox *grBox = new QGroupBox(module.getTypeMod()+"/Линия:"+module.getNumLine());
    QVBoxLayout *vBox = new QVBoxLayout();
    vBox->addLayout(greateHBox1(type));

    hBtn = new QHBoxLayout();
    createItems();

    vBox->addLayout(hBtn);
    vBox->addLayout(greateHBox2());
    grBox->setLayout(vBox);
    QHBoxLayout *frm = new QHBoxLayout();
    frm->addWidget(grBox);
    setLayout(frm);
    this->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                        "stop:0 #909090, stop: 0.5 #A0A0A0,stop: 0.6 #B0B0B0, stop:1 #C0C0C0)");
    //stateBtn(false);
    comWord = 0;
    readWord = 0;
    connect(btnCheckAll, SIGNAL(clicked()), this, SLOT(allOn()));
    connect(btnClear, SIGNAL(clicked()), this, SLOT(allOff()));

    if(type==2){
       connect(cmbMnc, SIGNAL(currentIndexChanged(int)), this, SLOT(setDataModNNC(int)));
       connect(spNumLine, SIGNAL(valueChanged(int)), this, SLOT(setNumLineNNC(int)));
    }

}

discretRDS::~discretRDS()
{

}

QHBoxLayout *discretRDS::greateHBox1(int type)
{
    QHBoxLayout *hBox1 = new QHBoxLayout();
    labelAddr = new QLabel("Адрес:");
    labelAddr->setFixedWidth(37);
    hBox1->addWidget(labelAddr);
    txtAddr = new QLineEdit(module.getAddrDecMod());
    txtAddr->setFixedWidth(60);
    txtAddr->setAlignment(Qt::AlignRight);
    txtAddr->setReadOnly(true);
    hBox1->addWidget(txtAddr);
    labelAddrHex = new QLabel("Hex:");
    labelAddrHex->setFixedWidth(26);
    hBox1->addWidget(labelAddrHex);
    txtAddrHex = new QLineEdit(module.getAddrHexMod());
    txtAddrHex->setFixedWidth(60);
    txtAddrHex->setAlignment(Qt::AlignRight);
    txtAddrHex->setReadOnly(true);
    hBox1->addWidget(txtAddrHex);
    labelPlo = new QLabel("Адрес ПЛО:");
    labelPlo->setFixedWidth(61);
    hBox1->addWidget(labelPlo);
    txtPlo = new QLineEdit(module.getAddrPLO());
    txtPlo->setFixedWidth(60);
    txtPlo->setAlignment(Qt::AlignRight);
    txtPlo->setReadOnly(true);
    hBox1->addWidget(txtPlo);
    if(type == 2){
       labelNNC = new QLabel("ННС:");
       hBox1->addWidget(labelNNC);
       cmbMnc = new QComboBox();
       cmbMnc->addItem("Выбор модуля ННС");
       for(DataModules dm : *nncList){
           QString s=dm.getTypeMod()+" Адрес:"+dm.getAddrHexMod();
           cmbMnc->addItem(s);
       }
       hBox1->addWidget(cmbMnc);
       labelSpLine = new QLabel("Линия:");
       hBox1->addWidget(labelSpLine);
       spNumLine = new QSpinBox();
       spNumLine->setRange(1,2);
       spNumLine->setValue(1);
       hBox1->addWidget(spNumLine);
       hBox1->addSpacing(100);
    }else if(type==1){
        hBox1->addSpacing(300);
    }

    labelWrite = new QLabel("Записано:");
    labelWrite->setFixedWidth(50);
    hBox1->addWidget(labelWrite);
    txtWrite = new QLineEdit("0");
    txtWrite->setAlignment(Qt::AlignRight);
    txtWrite->setReadOnly(true);
    txtWrite->setFixedWidth(60);
    hBox1->addWidget(txtWrite);
    labelRead = new QLabel("Прочитано:");
    labelRead->setFixedWidth(60);
    hBox1->addWidget(labelRead);
    txtRead = new QLineEdit("0");
    txtRead->setFixedWidth(60);
    txtRead->setAlignment(Qt::AlignRight);
    txtRead->setReadOnly(true);
    hBox1->addWidget(txtRead);

    return hBox1;
}

QHBoxLayout *discretRDS::greateHBox2()
{
    QHBoxLayout *hBox2 = new QHBoxLayout();
    btnClear = new QPushButton("Выключить все");
    btnClear->setFixedWidth(110);
    btnClear->setIcon(QIcon(":/cmbbImages/document_shred.png"));
    btnClear->setCursor(Qt::PointingHandCursor);
    hBox2->addWidget(btnClear);
    btnCheckAll = new QPushButton("Включить все");
    btnCheckAll->setFixedWidth(110);
    btnCheckAll->setIcon(QIcon(":/cmbbImages/check_box.png"));
    btnCheckAll->setCursor(Qt::PointingHandCursor);
    hBox2->addWidget(btnCheckAll);

    return hBox2;
}


void discretRDS::setDataModNNC(int idx)
{
    if(idx==0){
       modNNC={};
    }else{
          modNNC = nncList->at(idx-1);
    }
}

void discretRDS::setNumLineNNC(int line)
{
    lineNNC = line;
}

void discretRDS::createItems()
{
    for(quint32 i=0; i < 32; i++){
        QString s=ploHexToHex(txtPlo->text(), i);
        mapItems.insert(i, new DiscrItem(i, s,this));
    }

    for(quint32 j=0; j < 32; j++){
        hBtn->addLayout(mapItems.value(j)->getDiscrItem());
    }
}

void discretRDS::setStateBtn(bool b)
{
    for(quint32 i=0; i < 32; i++){
        mapItems.value(i)->setStateBtn(b);
        mapItems.value(i)->setColorItem(b);
    }
}

void discretRDS::stateBtn(bool b)
{
    btnCheckAll->setDisabled(b);
    btnClear->setDisabled(!b);
}
quint32 discretRDS::getReadWord() const
{
    return readWord;
}

void discretRDS::setReadWord(const quint32 &value)
{
    readWord = value;
}


QString discretRDS::ploHexToHex(QString strHex, quint8 idx)
{
    char buff[4];
    bool ok;
    quint16 val = strHex.trimmed().toInt(&ok,16)+idx;
    sprintf(buff,"%3X",val);
    return (QString(buff));
}


int discretRDS::getLineNNC() const
{
    return lineNNC;
}

DataModules discretRDS::getModNNC() const
{
    return modNNC;
}

DataModules discretRDS::getModule() const
{
    return module;
}
QLineEdit *discretRDS::getTxtWrite() const
{
    return txtWrite;
}

void discretRDS::setTxtWrite(QLineEdit *value)
{
    txtWrite = value;
}
QLineEdit *discretRDS::getTxtRead() const
{
    return txtRead;
}

void discretRDS::setTxtRead(QLineEdit *value)
{
    txtRead = value;
}



quint32 discretRDS::getComWord() const
{
    return comWord;
}

void discretRDS::setComWord(const quint32 &value)
{
    comWord = value;
    //qDebug()<<"comWord"<<comWord;
}


void discretRDS::allOn()
{
    setStateBtn(true);
    //stateBtn(true);
    comWord = 0XFFFFFFFF;
    //qDebug()<<"comWord"<<comWord;
}

void discretRDS::allOff()
{
    setStateBtn(false);
    //stateBtn(false);
    comWord = 0;
    //qDebug()<<"comWord"<<comWord;
}

void discretRDS::setIndItemRDS(quint32 wrd)
{
   for(int i=0; i<32; i++){
       quint32 tmp=1;
       tmp <<= i;
       bool state = wrd & tmp;
       mapItems.value(i)->setColorInd(state);
   }
}






