#include "testrds.h"
#include "ccmbb.h"
#include <QDebug>

QMutex mutex;
QWaitCondition wait;

TestRDS::TestRDS(QWidget *parent, Qt::WindowFlags f):QDialog(parent, f)
{
    mainWin = (MainWindow*)parent;
    sPorts = mainWin->ports;
    //rdsLine1 = new QList<discretRDS*>();
    //rdsLine2 = new QList<discretRDS*>();
    testRun = false;
    QVBoxLayout *vBox = new QVBoxLayout();
    vBoxRDS = new QVBoxLayout();
    vBoxBtn = new QVBoxLayout();
    QHBoxLayout *hBox = new QHBoxLayout();
    btnWrite = new QPushButton("Старт");
    btnWrite->setIcon(QIcon(":/cmbbImages/port1.png"));
    btnWrite->setCursor(Qt::PointingHandCursor);
    btnWrite->setFixedWidth(60);
    hBox->addWidget(btnWrite);
    btnStop = new QPushButton("Стоп");
    btnStop->setIcon(QIcon(":/cmbbImages/stop.png"));
    btnStop->setCursor(Qt::PointingHandCursor);
    btnStop->setFixedWidth(60);
    hBox->addWidget(btnStop);
    vBoxBtn->addLayout(hBox);
    vBox->addLayout(vBoxRDS);
    vBox->addLayout(vBoxBtn);
    setLayout(vBox);
    cntObj = 0;
    setStyleSheet(QString("background-color: rgb(200,200,200);"));
    setFixedWidth(400);
    stateBtn(false);

    tmr = new QTimer();
    tmr->setInterval(100);
    tmr->setTimerType(Qt::PreciseTimer);

    connect(btnWrite, SIGNAL(clicked()), this, SLOT(startWrite()));
    connect(btnStop,  SIGNAL(clicked()), this, SLOT(stopWrite()));
}

TestRDS::~TestRDS()
{
    if(cmdToRdsLine1) delete cmdToRdsLine1;
    if(cmdToRdsLine2) delete cmdToRdsLine2;
    if(thrTestLine1) delete thrTestLine1;
    if(thrTestLine2) delete thrTestLine2;
    if(tmr){delete tmr;}
    delete vBoxBtn;
    delete vBoxRDS;
    delete btnWrite;
    delete btnStop;
}

//Получим модули ННС /////////////////////////////////////////////////
QList<DataModules> TestRDS::getDataMnc()
{
    QList<DataModules> l;

    QList<DataModules> *list = mainWin->getListModul()->getDataMod();
    QListIterator<DataModules> it(*list);
    while(it.hasNext()){
        DataModules d = (DataModules)it.next();
        if(!d.getTypeMod().indexOf("ННС")){
            l.append(d);
        }
    }
    return l;
}

void TestRDS::addRDS(DataModules mod, int type)
{
    if(type==2 && listRDS.size()==1){
       vBoxRDS->addWidget(listRDS.at(cntObj-1));
       layout()->setSizeConstraint(QLayout::SetMinimumSize);
       return;
    }
    QList<DataModules> ls=getDataMnc();
    listRDS.append(new discretRDS(type, mod, ls, this));
    vBoxRDS->addWidget(listRDS.at(cntObj));
    layout()->setSizeConstraint(QLayout::SetMinimumSize);
    cntObj++;
}


void TestRDS::delItems()
{
    for(discretRDS *d: listRDS){
        delete d;
    }
    listRDS.clear();
    cntObj = 0;
}

void TestRDS::closeEvent(QCloseEvent *event)
{
    if(testRun){
       stopWrite();
       QThread::msleep(300);
    }

    delItems();
    mainWin->menuDisRDSNNS(false, false);
    event->accept();
}

void TestRDS::startWrite()
{
  stateBtn(true);
  testRun = true;
  for(discretRDS *d : listRDS){
      if(d->getModule().getNumLine().toInt()==1){
         rdsLine1.append(d);
      }else{
         rdsLine2.append(d);
      }
  }
  controlPorts(DIS_CONNECT_PORT);//Otключим порты основного потока//
  initThread();
  connect(tmr, SIGNAL(timeout()), cmdToRdsLine1, SLOT(startWriteToRDS()));
  connect(tmr, SIGNAL(timeout()), cmdToRdsLine2, SLOT(startWriteToRDS()));
  connect(thrTestLine1, &QThread::finished, cmdToRdsLine1, &QObject::deleteLater);
  connect(thrTestLine2, &QThread::finished, cmdToRdsLine2, &QObject::deleteLater);
  tmr->start();
}

void TestRDS::stopWrite()
{
    stateBtn(false);
    testRun = false;
    tmr->stop();
    closeThreadAll();
    rdsLine1.clear();
    rdsLine2.clear();
    controlPorts(CONNECT_PORT);//Подключим порты основного потока//
}


void TestRDS::testTimer()
{
    qDebug()<<"TestTimer";
}


//Управление портом для основного потока==============================//
void TestRDS::controlPorts(bool flag)
{
    if(flag){
        QThread::msleep(500);
        sPorts.at(0)->connectPort();
        sPorts.at(1)->connectPort();
        QThread::msleep(100);
        //startTestTM(0);
    }else{
        //startTestTM(test->N);
        QThread::msleep(500);
        sPorts.at(0)->disconnectPort();
        sPorts.at(1)->disconnectPort();
    }
}//===================================================================//

void TestRDS::stop_worker(QThread *tr)
{
    QThread::msleep(500);
    stopThread(tr);
    //btnVisible(test->disableStop);
}

//Остановим поток =====================================================//
bool TestRDS::stopThread(QThread *tr)
{
    bool retVal = true;
    int del = 0;

    if(tr->isRunning()){
       tr->exit();
    }
    while(!tr->wait(100)){
        if(del++ > 100){
            retVal = false;
            break;
        }
    }
    //qDebug()<<"thrTest->wait del:"<<del;
    return retVal;
}//=====================================================================//

void TestRDS::initThread()
{

    thrTestLine1 = new QThread();
    thrTestLine2 = new QThread();
    cmdToRdsLine1 = new CmdToRds(mainWin->namePort.at(0) ,rdsLine1);
    cmdToRdsLine2 = new CmdToRds(mainWin->namePort.at(1) ,rdsLine2);

    connect(cmdToRdsLine1, SIGNAL(sigReqPrt(DataModules *, int, discretRDS *)),
            cmdToRdsLine2, SLOT(slReqPort(DataModules *, int, discretRDS *)));
    connect(cmdToRdsLine2, SIGNAL(sigSendWord(ulong)), cmdToRdsLine1, SLOT(slSendWord(ulong)));
    connect(cmdToRdsLine2, SIGNAL(sigReqPrt(DataModules *, int, discretRDS *)),
            cmdToRdsLine1, SLOT(slReqPort(DataModules *, int, discretRDS *)));
    connect(cmdToRdsLine1, SIGNAL(sigSendWord(ulong)), cmdToRdsLine2, SLOT(slSendWord(ulong)));

    cmdToRdsLine1->moveToThread(thrTestLine1);
    cmdToRdsLine2->moveToThread(thrTestLine2);

    thrTestLine1->start();
    thrTestLine2->start();
}

void TestRDS::closeThreadAll()
{
    stop_worker(thrTestLine1);
    stop_worker(thrTestLine2);
}

void TestRDS::stateBtn(bool b)
{
    btnWrite->setDisabled(b);
    btnStop->setDisabled(!b);
}


