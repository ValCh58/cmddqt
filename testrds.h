#ifndef TESTRDS_H
#define TESTRDS_H

#include <QObject>
#include <QDialog>
#include <QList>
#include <QCloseEvent>
#include <QTimer>
#include <QWaitCondition>
#include <QMutex>

#include "datamodules.h"
#include "sport.h"
#include "discretrds.h"
#include "worker.h"
#include "comdtords.h"

#define CONNECT_PORT 1



#define DIS_CONNECT_PORT 0

class MainWindow;

extern QMutex mutex;
class TestRDS : public QDialog
{
    Q_OBJECT

public:
    explicit TestRDS(QWidget * parent=0, Qt::WindowFlags f=Qt::Drawer);
    ~TestRDS();

    void addRDS(DataModules mod, int type);
    CmdToRds *cmdToRdsLine1;
    CmdToRds *cmdToRdsLine2;
    QThread *thrTestLine1;
    QThread *thrTestLine2;
    QList<discretRDS*> listRDS;
    QList<discretRDS*> rdsLine1;//Модули линии 1//
    QList<discretRDS*> rdsLine2;//Модули линии 2//



private:
    MainWindow *mainWin;
    QList<SPort*> sPorts;
    QVBoxLayout *vBoxBtn;
    QVBoxLayout *vBoxRDS;
    void delItems();
    void closeEvent(QCloseEvent *event);

    int cntObj;
    QPushButton *btnWrite;//Записать команду
    QPushButton *btnStop;//Остановить запись команды

    QTimer *tmr;
    void controlPorts(bool flag);
    bool stopThread(QThread *tr);
    void stop_worker(QThread *tr);
    void initThread();
    void closeThreadAll();
    void stateBtn(bool b);
    bool testRun;
    QList<DataModules> getDataMnc();

private slots:
    void startWrite();
    void stopWrite();
    void testTimer();


signals:


};


#endif // TESTRDS_H
