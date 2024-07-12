#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QList>
#include <QToolBar>
#include <QStatusBar>
#include <QFile>
#include <QTableView>
#include <QSortFilterProxyModel>

#include "sport.h"
#include "viewmodules.h"
#include "cmbblistmodules.h"
#include "modules.h"
#include "tinyxml/tinyxml.h"
#include "cmbblistmodules.h"
#include "myprogressbar.h"
#include "logfile.h"
#include "calibratedialog.h"
#include "settingsanalog.h"
#include "rateconstants.h"
#include "settingmodule.h"
#include "about.h"
#include "testrds.h"
#include "calibrationmbac.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
    ViewModules *getViewmodul() const;
    CmbbListModules *getListModul() const;
    void setListModul(CmbbListModules *value);
    QList<SPort*> ports;
    QList<QString> namePort;
    QMap<QString, AdcPloAddr> *getMapAdcPloAddr() const;
    QMap<QString, Adcs> *getMapAdcs() const;
    void closeTest();
    void menuDisRDSNNS(bool setrds, bool setrdsnns);

private:
    //========Меню===================//
    //--Меню файл-------------------//
    QMenu *menuFile = nullptr;
    //QMenu *subMenuOpen;
     // QAction *actConfigModules;
     // QAction *actDescripAnalogSignals;
    QAction *actExit = nullptr;
    //--Меню модули-----------------//
    QMenu *menuModules = nullptr;
    QAction *actSearchModule = nullptr;
    QAction *actCheckTecModule = nullptr;
    QAction *actTestModule = nullptr;
    QAction *actTestRds = nullptr;
    QAction *actTestRdsHHC = nullptr;
    //--Меню сервис-----------------//
    QMenu *menuService = nullptr;
    QMenu *subMenuIndication = nullptr;
    QAction *actIndOn = nullptr;
    QAction *actIndOff = nullptr;
    QAction *actIndFlash = nullptr;
    QAction *actSync = nullptr;
    QAction *actConstSpeed = nullptr;
    //QAction *actControlACP;
    QAction *actResetID = nullptr;
    QAction *actResetKo = nullptr;
    QAction *actMBAC = nullptr;
    //QAction *actSettingModule;
    //--Меню помощь-----------------//
    QMenu *menuHelp;
    QAction *actAbout;
    //===============================//
    QAction *actCalibr = nullptr;
    QAction *actSetting = nullptr;
    QAction *actFindItem = nullptr;
    QAction *actSetpoint = nullptr;
    QToolBar *toolBar = nullptr;
    void createToolBar();
    void createActions();
    void createMenus();
    ViewModules * viewmodul = nullptr;
    CmbbListModules *listModul = nullptr;
    QSortFilterProxyModel *proxyModel = nullptr;
    int LINE_COUNT;
    QList<QByteArray> *bufBytesRead = nullptr;
    void initView();
    bool loadTypeModules(QString file);
    QList<Modules> *listTypeModules = nullptr;
    void loadModulesToModel();
    QString getTypeModule(int type);
    uint getColorModule(int type);
    QMap<QString,AdcPloAddr> *mapAdcPloAddr = nullptr;
    QMap<QString,Adcs> *mapAdcs = nullptr;
    void loadAdcPloAddr(QString strFile);
    void loadAdcs(QString fileName);
    QString getKeyAdcPlo(QString k1, QString k2);
    MyProgressBar *prg = nullptr;
    void activateProgress();
    void deactivateProgress();
    void commandControlInd(quint8 cmd);
    DataModules getItemDataProxyModel(QModelIndex modIdx);
    CalibrateDialog *calibr = nullptr;
    SettingsAnalog *setanalog = nullptr;
    RateConstants *rateConst = nullptr;
    SettingModule *setModule = nullptr;
    CalibrationMBAC *calibrationMBAC = nullptr;

    DataModules techModule;
    bool checkModules(DataModules module, quint8 command);
    bool setModeTested(DataModules data);

    TestRDS *testRDS = nullptr;
    TestRDS *testRDSNNS = nullptr;
    bool messageRDS(DataModules data);

private slots:
    //void configModules();
    //void descripAnalogSignals();
    void searchModule();
    void indOn();
    void indOff();
    void indFlash();
    void modeSync();
    void aBout();
    void makeCalibr();
    void makeFindItem();
    void makeSetPoint();
    void printError(QString strMess);
    void fillList(QByteArray ba);
    void checkTecModule(bool flag);
    void testModule();
    void constSpeed();
    void resetID();
    void resetKo();
    void showContextMenu(const QPoint &point);
    void settingModule();
    void testRds();
    void testRdsNnc();
    void calibrationAdcMbac();

signals:
    void sigWritePort0(SPort*, int, int, int);
    void sigWritePort1(SPort*, int, int, int);
    void sigReadPortAll0(SPort*, int, int, int);
    void sigReadPortAll1(SPort*, int, int, int);
    //Обнаружить модуль////////////////////////////
    void sigMakeFindItem0(SPort*, int, int);
    void sigMakeFindItem1(SPort*, int, int);
    //Сигнал на получение ответа от ТМ и ОК////////
    void sigCheckModules0(SPort*, int, int, bool*);
    void sigCheckModules1(SPort*, int, int, bool*);
    //-------------------------------------------//
    void startProgress();
    void stopProgress();
    void changeProgress(int);

};

extern bool Assert_x(bool flag, QString title, QString message);
extern bool XmlError(TiXmlDocument* cfg, const char *filename, char* buff);

#endif // MAINWINDOW_H
