#ifndef TESTMODULE_H
#define TESTMODULE_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QListWidgetItem>
#include <QPlainTextDocumentLayout>
#include <QToolButton>
#include <QTextFrame>

//#include "ccmbb.h"
#include "datamodules.h"
#include "sport.h"
#include "test.h"
#include "worker.h"
#include "testtimer.h"
#include "testuart1.h"
#include "testuart0.h"
#include "testled.h"
#include "checkknoawo.h"
#include "testdiscrnbc.h"
#include "testcheckfain1.h"
#include "testacp.h"
#include "testcalibrch.h"
#include "testdiscrnnc.h"
#include <QThread>
#include <QTimer>
#include <QSerialPortInfo>
#include <QTableWidget>


#define BTNPREV  0x01
#define BTNNEXT  0x02
#define BTNSTART 0x04
#define BTNSTOP  0x08
#define BTNREADY 0x10
#define BTNYES   0x20
#define BTNNO    0x40
#define BTNCLEAR 0x80

#define CONNECT_PORT 1
#define DIS_CONNECT_PORT 0

class MainWindow;

class TestModule : public QDialog
{
    Q_OBJECT

public:
    TestModule(DataModules *data, DataModules *datatech, QWidget * parent = 0, Qt::WindowFlags f = Qt::Drawer);
    ~TestModule();
    Tests* tests;
    Tests* selTests;
    bool getSkipped() const;
    MainWindow *mainWin;
    //Для теста потока!!!
    Worker *testModule;
    QThread *thrTest;
    QTimer *tm;
    bool stopThread();
    //+!!!!!!!!!!!!!!!!!!

private:
    QMap<int, QIcon> mapIcon;
    QList<SPort*> sPorts;
    DataModules *modul;
    DataModules *modulTech;
    void init();
    QListWidget *list;
    QGroupBox *makeMenu();
    QGroupBox *makeFormTest(int width);
    void loadXMLTests(QString file);
    void initMapIcon();
    QIcon getIcon(int numImg);
    void initTestModules();
    void insertItemList(Test *t);
    int cntOK;
    int cntInterrupt;
    int cntBad;
    QVBoxLayout *formBox;
    void makeFormTestTimer();
    void makeFormTestLamp();
    void makeFormTestUART1();
    void makeFormTestUART0();
    void makeFormTestKNO_AWO();
    void makeFormTestCalibACP1();
    void makeFormTestCalibACP2();
    void makeFormTestCheckACP1();
    void makeFormTestCheckACP2();
    void makeFormTestDiscrNNC();
    void makeFormTestDiscrHBC();
    void makeFormTestDiscrPDC();
    void makeFormTestDiscrKDC();
    void makeFormTestCheckFAIN1();
    void makeFormTestCheckFAIN2();
    void closeEvent(QCloseEvent *event);
    void btnVisible(const quint8 mask) const;
    void controlPorts(bool flag);
    void startTestTM(int cmdN);
    QByteArray *retInfo;
    bool skipped;
    QString printResultTestTimer(QByteArray *bArr);
    QLabel *labNum;
    QLabel *lcdNum;
    QTimer *timer;
    uint cntTime;
    void timerStartForTest();

    QList<QTextTable*> listTxtTblAcp;
    QTextTableFormat getTableFormat(QColor color, Qt::Alignment align);
    QTextCharFormat  getCharFormat(QString nameFont, QFont::Weight weight, int sizeFont, QBrush foreGrnd, QBrush backGrnd);

    void makeTestAcp(Worker *w);
    void makeTestNNC_KDC(Worker *w);
    void makeTestUART0_1(Worker *w);

//Представление области теста////////////

    //QHBoxLayout *btnLayout;
    QToolButton *btnPrev;
    QToolButton *btnNext;
    QToolButton *btnStart;
    QToolButton *btnStop;
    QToolButton *btnReady;
    QToolButton *btnYes;
    QToolButton *btnNo;
    QToolButton *btnClear;
    QToolButton *btnRep;

    QLabel *lbTries;
    QLineEdit *txTries;
    QLabel *lbFails;
    QLineEdit *txFails;

    QVBoxLayout *msgLayout;
    QTextEdit *txtEdit;
    void prepareTest(QString caption, QString abstract);
    void prnMessageTest(QString msg);
    Test *getTestNumber(int idx);
    Test *test;
    void makeResult();
    void chekResult(bool val, int code, Test *test);
    void printColorToTextEdit(QString str, QBrush brush, int szFont, QString nmFont="Areal");
    QTableWidget *tWidget;
    void insertTable(int row);
    QTextTable *table;
    QTextTable *tableDiscret;
    QTextTable *tableFain;
    void insertTableDiscret(int col);
    void setSignal(ulong buf, int row, int idx);
    void setItemCell(QTextTable *tTable, int row, int col, QString s, QTextCharFormat frm);
    void setItemCell(QTextTable *tTable, int row, int col, QString s, QTextCharFormat frm, Qt::Alignment alignment);
    QString getColorName(QTextTable *tTable, int row, int col);

    QHBoxLayout *createBtnLayout();
    QHBoxLayout *createStatistLayout();
    QVBoxLayout *createMsgLayout();
    QToolButton *makeToolButton(QIcon ico, QString str)const;

//======================================//


private slots:
    void runTest(QModelIndex idx);
    void startTest();
    void clearPrepareTest();
    //++++++++++++++++++++++++
    void stopWorker();
    void stop_worker();
    void stopWorkerLed();
    void calcRetInfo(bool complit, bool user_actions, bool error_cycle, QByteArray info);
    void showResultCnt(int trise, int fails);
    void calcRetInfo(bool error_cycle, int info);
    void calcRetInfo();
    void currSetCnt(int cnt);
    void stopWorkerYes();
    void stopWorkerNo();
    //++++++++++++++++++++++++
    void testTime();
    void printMsgFromThrd(QString msg, int color);
    void stateBtn(quint8 flagDisable);
    void changeUserReady();
    void makeTableDiscret(int col);
    void setSignals(ulong bufTm, ulong bufOk, int columns);
    void checkSignals(ulong buf1, ulong buf2, int index);
    void discrError();
    void resultTest();
    void insertTableACP(int AcpType, int ch);
    void makeTextTableAcp(QString header, QString name);
    void getDataFromFAIN(QString color, QString val, int idxTbl);
    void makeReport();

signals:
    void sigStartTM0(SPort*, int, int, int);
    void sigStartTM1(SPort*, int, int, int);
    void sigRunTest();
    void sigRunTest(uint*);
    void sigRunTestUART1();
    void sigTermTestTimer(bool);
    void sigProgStop();
    void sigSetUserTerminate(bool);

};

#endif // TESTMODULE_H
