#include "rateconstants.h"
#include "ccmbb.h"
#include "QThread"

RateConstants::RateConstants(DataModules *data, QWidget *parent, Qt::WindowFlags f):QDialog(parent, f)
{
    setWindowIcon(QIcon(":/cmbbImages/construction.png"));
    MainWindow *mainWin = (MainWindow*)parent;
    sPorts = mainWin->ports;
    modul = data;
    init();
    connect(this, SIGNAL(sigReadRateConst0(DataModules*,uint*,uint*)), sPorts.at(0), SLOT(ReadRateConst(DataModules*,uint*,uint*)));
    connect(this, SIGNAL(sigReadRateConst1(DataModules*,uint*,uint*)), sPorts.at(1), SLOT(ReadRateConst(DataModules*,uint*,uint*)));
    readConst();
    setWindowTitle("Константы скорости " + modul->getTypeMod()+" "+modul->getIdMod());
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());

    connect(btCancel, SIGNAL(pressed()), this, SLOT(close()));
    connect(btWrite, SIGNAL(pressed()), this, SLOT(writeRateConst()));
    connect(this, SIGNAL(sigWriteRateConst0(DataModules*,uint*,uint*)), sPorts.at(0), SLOT(WriteRateConst(DataModules*,uint*,uint*)));
    connect(this, SIGNAL(sigWriteRateConst1(DataModules*,uint*,uint*)), sPorts.at(1), SLOT(WriteRateConst(DataModules*,uint*,uint*)));

}

RateConstants::~RateConstants()
{

}

//Чтение констант скорости///////////////////////////////////////////////////////////////////////////
void RateConstants::readConst()
{
    int numCom = modul->getNumLine().toInt()-1;//Номер линии//

    switch(numCom){
    case 0:
        emit sigReadRateConst0(modul, arCh, arFx);
        break;
    case 1:
        emit sigReadRateConst1(modul, arCh, arFx);
        break;
    }
    fillForm();
}

void RateConstants::fillForm()
{
    editCh1->setText(QString::number(arCh[0]));
    editFx1->setText(QString::number(arFx[0]));
    editCh2->setText(QString::number(arCh[1]));
    editFx2->setText(QString::number(arFx[1]));
    if(modul->getTypeModule()==4){
        editCh3->setText(QString::number(arCh[2]));
        editFx3->setText(QString::number(arFx[2]));
        editCh4->setText(QString::number(arCh[3]));
        editFx4->setText(QString::number(arFx[3]));
        editCh5->setText(QString::number(arCh[4]));
        editFx5->setText(QString::number(arFx[4]));
        editCh6->setText(QString::number(arCh[5]));
        editFx6->setText(QString::number(arFx[5]));
        editCh7->setText(QString::number(arCh[6]));
        editFx7->setText(QString::number(arFx[6]));
        editCh8->setText(QString::number(arCh[7]));
        editFx8->setText(QString::number(arFx[7]));
    }
    if(modul->getTypeModule()!=4){
        editCh3->setDisabled(true);
        editFx3->setDisabled(true);
        editCh4->setDisabled(true);
        editFx4->setDisabled(true);
        editCh5->setDisabled(true);
        editFx5->setDisabled(true);
        editCh6->setDisabled(true);
        editFx6->setDisabled(true);
        editCh7->setDisabled(true);
        editFx7->setDisabled(true);
        editCh8->setDisabled(true);
        editFx8->setDisabled(true);
    }

}

//Запись констант скорости//////////////////////////////////////////////////////////////////////////////
void RateConstants::writeRateConst()
{
    setDisBtn(true);
    repaint();
    readFromArrSet();
    vProgrBar->progress->setValue(50);
    repaint();
    int numCom = modul->getNumLine().toInt()-1;//Номер линии//

    switch(numCom){
    case 0:
        emit sigWriteRateConst0(modul, arCh, arFx);
        break;
    case 1:
        emit sigWriteRateConst1(modul, arCh, arFx);
        break;
    }
    vProgrBar->progress->setValue(75);
    repaint();
    QThread::usleep(4000*1000);
    //vProgrBar->progress->setValue(80);
    readConst();
    vProgrBar->progress->setValue(100);
    setDisBtn(false);

}

void RateConstants::setDisBtn(bool flag)
{
    btWrite->setDisabled(flag);
    btCancel->setDisabled(flag);
}

void RateConstants::readFromArrSet()
{
    arCh[0] = editCh1->text().toUInt();
    arFx[0] = editFx1->text().toUInt();
    arCh[1] = editCh2->text().toUInt();
    arFx[1] = editFx2->text().toUInt();
    if(modul->getTypeModule() == 4){
        arCh[2] = editCh3->text().toUInt();
        arFx[2] = editFx3->text().toUInt();
        arCh[3] = editCh4->text().toUInt();
        arFx[3] = editFx4->text().toUInt();
        arCh[4] = editCh5->text().toUInt();
        arFx[4] = editFx5->text().toUInt();
        arCh[5] = editCh6->text().toUInt();
        arFx[5] = editFx6->text().toUInt();
        arCh[6] = editCh7->text().toUInt();
        arFx[6] = editFx7->text().toUInt();
        arCh[7] = editCh8->text().toUInt();
        arFx[7] = editFx8->text().toUInt();
    }

}

void RateConstants::init()
{
    QVBoxLayout *vBoxL = new QVBoxLayout();
    labCh1 = new QLabel("Скорость изменения:");
    editCh1 = new QLineEdit();
    editCh1->setFixedWidth(60);
    editCh1->setAlignment(Qt::AlignRight);
    labFx1 = new QLabel("Время фиксации:");
    editFx1 = new QLineEdit();
    editFx1->setFixedWidth(60);
    editFx1->setAlignment(Qt::AlignRight);
    vBoxL->addWidget(makeGroupBox("АЦП-1",labCh1,editCh1,labFx1,editFx1));
    labCh2 = new QLabel("Скорость изменения:");
    editCh2 = new QLineEdit();
    editCh2->setFixedWidth(60);
    editCh2->setAlignment(Qt::AlignRight);
    labFx2 = new QLabel("Время фиксации:");
    editFx2 = new QLineEdit();
    editFx2->setFixedWidth(60);
    editFx2->setAlignment(Qt::AlignRight);
    vBoxL->addWidget(makeGroupBox("АЦП-2",labCh2,editCh2,labFx2,editFx2));
    labCh3 = new QLabel("Скорость изменения:");
    editCh3 = new QLineEdit();
    editCh3->setFixedWidth(60);
    editCh3->setAlignment(Qt::AlignRight);
    labFx3 = new QLabel("Время фиксации:");
    editFx3 = new QLineEdit();
    editFx3->setFixedWidth(60);
    editFx3->setAlignment(Qt::AlignRight);
    vBoxL->addWidget(makeGroupBox("АЦП-3",labCh3,editCh3,labFx3,editFx3));
    labCh4 = new QLabel("Скорость изменения:");
    editCh4 = new QLineEdit();
    editCh4->setFixedWidth(60);
    editCh4->setAlignment(Qt::AlignRight);
    labFx4 = new QLabel("Время фиксации:");
    editFx4 = new QLineEdit();
    editFx4->setFixedWidth(60);
    editFx4->setAlignment(Qt::AlignRight);
    vBoxL->addWidget(makeGroupBox("АЦП-4",labCh4,editCh4,labFx4,editFx4));
    QVBoxLayout *vBoxR = new QVBoxLayout();
    labCh5 = new QLabel("Скорость изменения:");
    editCh5 = new QLineEdit();
    editCh5->setFixedWidth(60);
    editCh5->setAlignment(Qt::AlignRight);
    labFx5 = new QLabel("Время фиксации:");
    editFx5 = new QLineEdit();
    editFx5->setFixedWidth(60);
    editFx5->setAlignment(Qt::AlignRight);
    vBoxR->addWidget(makeGroupBox("АЦП-5",labCh5,editCh5,labFx5,editFx5));
    labCh6 = new QLabel("Скорость изменения:");
    editCh6 = new QLineEdit();
    editCh6->setFixedWidth(60);
    editCh6->setAlignment(Qt::AlignRight);
    labFx6 = new QLabel("Время фиксации:");
    editFx6 = new QLineEdit();
    editFx6->setFixedWidth(60);
    editFx6->setAlignment(Qt::AlignRight);
    vBoxR->addWidget(makeGroupBox("АЦП-6",labCh6,editCh6,labFx6,editFx6));
    labCh7 = new QLabel("Скорость изменения:");
    editCh7 = new QLineEdit();
    editCh7->setFixedWidth(60);
    editCh7->setAlignment(Qt::AlignRight);
    labFx7 = new QLabel("Время фиксации:");
    editFx7 = new QLineEdit();
    editFx7->setFixedWidth(60);
    editFx7->setAlignment(Qt::AlignRight);
    vBoxR->addWidget(makeGroupBox("АЦП-7",labCh7,editCh7,labFx7,editFx7));
    labCh8 = new QLabel("Скорость изменения:");
    editCh8 = new QLineEdit();
    editCh8->setFixedWidth(60);
    editCh8->setAlignment(Qt::AlignRight);
    labFx8 = new QLabel("Время фиксации:");
    editFx8 = new QLineEdit();
    editFx8->setFixedWidth(60);
    editFx8->setAlignment(Qt::AlignRight);
    vBoxR->addWidget(makeGroupBox("АЦП-8",labCh8,editCh8,labFx8,editFx8));
    QHBoxLayout *hBoxLR = new QHBoxLayout();
    hBoxLR->addLayout(vBoxL);
    hBoxLR->addLayout(vBoxR);

    QHBoxLayout *hBoxBtn = new QHBoxLayout();
    btWrite = new QPushButton("Записать");
    btCancel = new QPushButton("Выйти");
    hBoxBtn->addWidget(btWrite);
    hBoxBtn->addWidget(btCancel);

    QVBoxLayout *vMainBox = new QVBoxLayout();
    vMainBox->addLayout(hBoxLR);
    vMainBox->addLayout(hBoxBtn);
    sbar = new QStatusBar(this);//Добавим статус бар//
    sbar->setFixedHeight(10);
    vProgrBar = new MyProgressBar(sbar);
    vProgrBar->progress->show();
    vProgrBar->progress->setValue(0);
    sbar->addWidget(vProgrBar);
    vMainBox->addWidget(sbar);

    setLayout(vMainBox);
}

QGroupBox *RateConstants::makeGroupBox(QString name, QLabel *labCh, QLineEdit *editCh, QLabel *labFx, QLineEdit *editFx)
{
    QGroupBox *gBox = new QGroupBox(name);
    QFormLayout *formLay = new QFormLayout();
    formLay->addRow(labCh, editCh);
    formLay->addRow(labFx, editFx);
    gBox->setLayout(formLay);
    return gBox;
}


