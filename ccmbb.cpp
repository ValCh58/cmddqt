#include <QVBoxLayout>
#include <QMessageBox>
#include <iostream>
#include <QStringList>
#include <QDebug>

#include "ccmbb.h"
#include "protocol.h"
#include "settingsanalog.h"
#include "Style.h"
#include "testmodule.h"


#define RDS     1
#define RDS_NNS 2


/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    //setAttribute(Qt::WA_DeleteOnClose);
    LINE_COUNT = 2;
    bufBytesRead    = new QList<QByteArray>();//Буфер для хранения принятых байт//
    listTypeModules = new QList<Modules>();//Типы модулей//
    mapAdcPloAddr  = new QMap<QString, AdcPloAddr>();//ПЛО модулей и начальные адреса АЦП//
    mapAdcs = new QMap<QString, Adcs>();//Загрузка аналоговых параметров//
    setWindowIcon(QIcon(":/cmbbImages/network_adapter.png"));
    createActions();
    createMenus();
    createToolBar();
    initView();
    loadTypeModules("modules.xml");
    loadAdcPloAddr("modules.cfg");
    loadAdcs("analog.cfg");
    viewmodul->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(viewmodul, &QTableView::customContextMenuRequested, this, &MainWindow::showContextMenu);

}

//Вызов контекстного меню//
/**
 * @brief MainWindow::showContextMenu
 * @param point
 */
void MainWindow::showContextMenu(const QPoint &point){
    Q_UNUSED(point);
    QMenu *menu = new QMenu();
    menu->addAction(actSetting);
    menu->addAction(actFindItem);
    menu->addAction(actSync);
    menu->addAction(actCalibr);
    menu->addAction(actSetpoint);
    menu->addAction(actConstSpeed);
    //menu->addAction(actControlACP);
    menu->addSeparator();
    menu->addAction(actResetID);
    menu->addAction(actResetKo);
    menu->addSeparator();
    menu->addAction(actCheckTecModule);
    menu->addAction(actTestModule);
    menu->addSeparator();
    menu->addAction(actTestRds);
    menu->addAction(actTestRdsHHC);
    menu->addAction(actMBAC);
    menu->exec(QCursor::pos());
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    if(bufBytesRead) delete bufBytesRead;
    if(mapAdcs) delete mapAdcs;

    if(proxyModel) delete proxyModel;
    if(listModul) delete listModul;
    if(viewmodul) delete viewmodul;

    if(mapAdcPloAddr) delete mapAdcPloAddr;
    if(listTypeModules) delete listTypeModules;
    if(menuFile) delete menuFile;
    if(menuModules) delete menuModules;
    if(subMenuIndication) delete subMenuIndication;
    if(menuService) delete menuService;
    if(menuHelp) delete menuHelp;
    if(actExit) delete actExit;
    if(actSearchModule) delete actSearchModule;
    if(actCheckTecModule) delete actCheckTecModule;
    if(actTestModule) delete actTestModule;
    if(actTestRds) delete actTestRds;
    if(actTestRdsHHC) delete actTestRdsHHC;

    if(actIndOn) delete actIndOn;
    if(actIndOff) delete actIndOff;
    if(actIndFlash) delete actIndFlash;
    if(actSync) delete actSync;
    if(actConstSpeed) delete actConstSpeed;
    if(actResetID) delete actResetID;
    if(actResetKo) delete actResetKo;
    if(actMBAC) delete actMBAC;

    if(actAbout) delete actAbout;
    if(actCalibr) delete actCalibr;
    if(actSetting) delete actSetting;
    if(actFindItem) delete actFindItem;
    if(actSetpoint) delete actSetpoint;
    if(toolBar) delete toolBar;
    if(calibrationMBAC) delete calibrationMBAC;
    if(testRDS) delete testRDS;
    if(testRDSNNS) delete testRDSNNS;

    if(calibrationMBAC) delete calibrationMBAC;
    if(calibr) delete calibr;
    if(setanalog) delete setanalog;
    if(rateConst) delete rateConst;
    if(setModule) delete setModule;
 }

/**
 * @brief MainWindow::activateProgress
 */
void MainWindow::activateProgress()
{
    prg = new MyProgressBar(this, 0, 60);

    //--------------------------------------------------------------//
    connect(this, SIGNAL(startProgress()), prg, SLOT(showProgress()));
    connect(this, SIGNAL(stopProgress()), prg, SLOT(hideProgress()));
    connect(this, SIGNAL(changeProgress(int)), prg, SLOT(nextStep(int)));
}

/**
 * @brief MainWindow::deactivateProgress
 */
void MainWindow::deactivateProgress()
{
    //--------------------------------------------------------------//
    disconnect(this, SIGNAL(startProgress()), prg, SLOT(showProgress()));
    disconnect(this, SIGNAL(stopProgress()), prg, SLOT(hideProgress()));
    disconnect(this, SIGNAL(changeProgress(int)), prg, SLOT(nextStep(int)));
    delete prg;
}

/**
 * @brief MainWindow::createToolBar
 */
void MainWindow::createToolBar()
{
    toolBar = addToolBar(tr("Файл"));
    toolBar->setFixedHeight(48);
    toolBar->addAction(actSearchModule);
    toolBar->addAction(actCalibr);
    toolBar->addAction(actSetting);
    toolBar->addAction(actFindItem);
    toolBar->addAction(actSetpoint);
    toolBar->addAction(actIndOn);
    toolBar->addAction(actIndOff);
    toolBar->addAction(actIndFlash);
    toolBar->addAction(actExit);
}

/**
 * @brief MainWindow::createActions
 */
void MainWindow::createActions(){
    //======Меню файл=========================================================================================//
    /* --Kонфигурация модулей----------------------------------------------------------------------------------//
    actConfigModules = new QAction(QIcon(":/cmbbImages/setting_tools.png"), tr("Конфигурация модулей"), this);
    actConfigModules->setStatusTip(tr("Конфигурация модулей"));
    connect(actConfigModules, SIGNAL(triggered()), this, SLOT(configModules()));*/
    /* --Описание аналоговых сигналов--------------------------------------------------------------------------//
    actDescripAnalogSignals = new QAction(QIcon(":/cmbbImages/system_monitor.png"), tr("Описание аналоговых сигналов"), this);
    actDescripAnalogSignals->setStatusTip(tr("Описание аналоговых сигналов"));
    connect(actDescripAnalogSignals, SIGNAL(triggered()), this, SLOT(descripAnalogSignals()));*/
    //--Выход-------------------------------------------------------------------------------------------------//
    actExit = new QAction(QIcon(":/cmbbImages/door_in.png"), tr("Выход"), this);
    actExit->setStatusTip(tr("Выход из программы"));
    connect(actExit, SIGNAL(triggered()), this, SLOT(close()));
    //=====Модули=============================================================================================//
    //--Поиск-------------------------------------------------------------------------------------------------//
    actSearchModule = new QAction(QIcon(":/cmbbImages/find.png"), tr("Поиск"), this);
    actSearchModule->setStatusTip(tr("Поиск модулей"));
    connect(actSearchModule, SIGNAL(triggered()), this, SLOT(searchModule()));

    actCheckTecModule = new QAction(tr("Технологический модуль"), this);//Установить чек при выборе элемента меню!!!
    actCheckTecModule->setCheckable(true);
    actCheckTecModule->setStatusTip(tr("Технологический модуль"));
    connect(actCheckTecModule, SIGNAL(triggered(bool)), this, SLOT(checkTecModule(bool)));

    actTestModule = new QAction(QIcon(":/cmbbImages/setting_tools.png"), tr("Тест модуля"), this);
    actTestModule->setStatusTip(tr("Тест модуля"));
    connect(actTestModule, SIGNAL(triggered()), this, SLOT(testModule()));

    actTestRds = new QAction(QIcon(":/cmbbImages/stethoscope.png"), tr("Тест модуля РДС"), this);
    connect(actTestRds, SIGNAL(triggered()), this, SLOT(testRds()));
    actTestRdsHHC = new QAction(QIcon(":/cmbbImages/stethoscope.png"), tr("Тест модуля РДС+ННС"), this);
    connect(actTestRdsHHC, SIGNAL(triggered()), this, SLOT(testRdsNnc()));
    //=====Сервис/Индикация===================================================================================//
    //--...включить-------------------------------------------------------------------------------------------//
    actIndOn = new QAction(QIcon(":/cmbbImages/i1.png"), tr("...включить"), this);
    actIndOn->setStatusTip(tr("Включить индикацию"));
    connect(actIndOn, SIGNAL(triggered()), this, SLOT(indOn()));
    //--...выключить------------------------------------------------------------------------------------------//
    actIndOff = new QAction(QIcon(":/cmbbImages/i2.png"), tr("...выключить"), this);
    actIndOff->setStatusTip(tr("Выключить индикацию"));
    connect(actIndOff, SIGNAL(triggered()), this, SLOT(indOff()));
    //--...переменно------------------------------------------------------------------------------------------//
    actIndFlash = new QAction(QIcon(":/cmbbImages/i3.png"), tr("...переменно"), this);
    actIndFlash->setStatusTip(tr("Индикацию переменно"));
    connect(actIndFlash, SIGNAL(triggered()), this, SLOT(indFlash()));
    //--Синхронизировать--------------------------------------------------------------------------------------//
    actSync = new QAction(QIcon(":/cmbbImages/database_gear.png"), tr("Синхронизировать"), this);
    actSync->setStatusTip(tr("Синхронизировать"));
    connect(actSync, SIGNAL(triggered()), this, SLOT(modeSync()));
    //Константы скорости--------------------------------------------------------------------------------------//
    actConstSpeed = new QAction(QIcon(":/cmbbImages/construction.png"), tr("Константы скорости"), this);
    actConstSpeed->setStatusTip("Константы скорости");
    connect(actConstSpeed, SIGNAL(triggered()), this, SLOT(constSpeed()));
    //Настройка модулей---------------------------------------------------------------------------------------//
    //actSettingModule = new QAction(QIcon(":/cmbbImages/setting_tools.png"), tr("Настройка модулей"), this);
    //actSettingModule->setStatusTip(tr("Настройка модулей"));
    //connect(actSettingModule, SIGNAL(triggered()), this, SLOT(settingModule()));
    //Контроль АЦП--------------------------------------------------------------------------------------------//
    //actControlACP = new QAction(QIcon(":/cmbbImages/convert_color_to_gray.png"), tr("Контроль АЦП"), this);
    //actControlACP->setStatusTip(tr("Контроль АЦП"));
    //connect(actControlACP, SIGNAL(triggered()), this, SLOT(controlACP()));
    //Настройка MBAC-- 22-05-2023 ----------------------------------------------------------------------------//
    actMBAC = new QAction(QIcon(":/cmbbImages/discr.png"), tr("Настройка модуля МВАС"), this);
    actMBAC->setStatusTip("Калибровка модуля МВАС");
    connect(actMBAC, SIGNAL(triggered()), this, SLOT(calibrationAdcMbac()));
    //Сброс и установка нового ID-----------------------------------------------------------------------------//
    actResetID = new QAction(QIcon(":/cmbbImages/hand_fuck.png"), tr("Сброс и установка нового ID"), this);
    actResetID->setStatusTip(tr("Сброс и установка нового ID"));
    connect(actResetID, SIGNAL(triggered()), this, SLOT(resetID()));
    //Сброс и установка калибровочных коэффициентов-----------------------------------------------------------//
    actResetKo = new QAction(QIcon(":/cmbbImages/hand_point.png"), tr("Сброс и установка калибровочных коэффициентов"), this);
    actResetKo->setStatusTip(tr("Сброс калибровочных коэффициентов"));
    connect(actResetKo, SIGNAL(triggered()), this, SLOT(resetKo()));
    //=====Помощь=============================================================================================//
    //--О программе-------------------------------------------------------------------------------------------//
    actAbout = new QAction(QIcon(":/cmbbImages/ask_and_answer.png"), tr("О программе"), this);
    actAbout->setStatusTip(tr("О программе"));
    connect(actAbout, SIGNAL(triggered()), this, SLOT(aBout()));
    //========================================================================================================//
    //==ToolBar Actions=======================================================================================//
    actCalibr = new QAction(QIcon(":/cmbbImages/calibr.png"), tr("Калибровка"), this);
    actCalibr->setStatusTip(tr("Калибровка"));
    connect(actCalibr, SIGNAL(triggered()), this, SLOT(makeCalibr()));
    actSetting = new QAction(QIcon(":/cmbbImages/settings2.png"), tr("Настройка модуля"), this);
    actSetting->setStatusTip(tr("Настройка модуля"));
    connect(actSetting, SIGNAL(triggered()), this, SLOT(settingModule()));
    actFindItem = new QAction(QIcon(":/cmbbImages/ind1.png"), tr("Обнаружить модуль"), this);
    actFindItem->setStatusTip(tr("Обнаружить модуль"));
    connect(actFindItem, SIGNAL(triggered()), this, SLOT(makeFindItem()));
    actSetpoint = new QAction(QIcon(":/cmbbImages/ustav.png"), tr("Задать уставки"), this);
    actSetpoint->setStatusTip(tr("Задать уставки"));
    connect(actSetpoint, SIGNAL(triggered()), this, SLOT(makeSetPoint()));
    //========================================================================================================//
}

/**
 * @brief MainWindow::createMenus
 */
void MainWindow::createMenus(){
    //==Меню файл====================================//
    menuFile = menuBar()->addMenu(tr("Файл"));
    //subMenuOpen = menuFile->addMenu(QIcon(":/cmbbImages/application_edit.png"), tr("Открыть"));
    //subMenuOpen->addAction(actConfigModules);
    //subMenuOpen->addAction(actDescripAnalogSignals);
    menuFile->addSeparator();
    menuFile->addAction(actExit);
    //==Меню модули==================================//
    menuModules = menuBar()->addMenu(tr("Модули"));
    menuModules->addAction(actSearchModule);
    menuModules->addAction(actFindItem);
    menuModules->addSeparator();
    menuModules->addAction(actCheckTecModule);
    menuModules->addAction(actTestModule);
    //==Меню сервис==================================//
    menuService = menuBar()->addMenu(tr("Сервис"));
    subMenuIndication = menuService->addMenu(QIcon(":/cmbbImages/lightbulb.png"), tr("Индикация"));
    subMenuIndication->addAction(actIndOn);
    subMenuIndication->addAction(actIndOff);
    subMenuIndication->addAction(actIndFlash);
    menuService->addAction(actSync);
    menuService->addAction(actCalibr);
    menuService->addAction(actSetpoint);
    menuService->addAction(actConstSpeed);
    //menuService->addAction(actControlACP);
    menuService->addAction(actResetID);
    menuService->addAction(actResetKo);
    //==Меню помощь==================================//
    menuHelp = menuBar()->addMenu(tr("Помощь"));
    menuHelp->addAction(actAbout);
    //===============================================//
    statusBar()->showMessage(tr("Готов"));
}

/**
 * @brief MainWindow::initView
 */
void MainWindow::initView()
{
    viewmodul = new ViewModules(this);
    viewmodul->setAlternatingRowColors(true);
    viewmodul->setStyle(new Style());
    setCentralWidget(viewmodul);
    listModul = new CmbbListModules(viewmodul);
    proxyModel = new QSortFilterProxyModel(viewmodul);
    proxyModel->setSourceModel(listModul);
    viewmodul->setModel(proxyModel);
    viewmodul->setSortingEnabled(true);
    proxyModel->sort(0,Qt::AscendingOrder);
    viewmodul->headerSize();
}

/**
 * @brief MainWindow::getListModul
 * @return
 */
CmbbListModules *MainWindow::getListModul() const
{
    return listModul;
}

/**
 * @brief MainWindow::setListModul
 * @param value
 */
void MainWindow::setListModul(CmbbListModules *value)
{
    listModul = value;
}

/**
 * @brief MainWindow::getViewmodul
 * @return
 */
ViewModules *MainWindow::getViewmodul() const
{
    return viewmodul;
}

//Загрузка данных по типам модулей из файла XML в QList Modules///////////////////////////
/**
 * @brief MainWindow::loadTypeModules
 * @param file
 * @return
 */
bool MainWindow::loadTypeModules(QString file)
{
    int i=0;
    char buf[256];
    TiXmlDocument* cfg;
    TiXmlElement* cfg_root;
    TiXmlElement* e;
    TiXmlElement* n;
    QByteArray ba = file.toUtf8();//QString
    const char *sFile = ba.data();// в char*

    //Загрузка файла описания типов модулей//
    cfg = new TiXmlDocument(sFile);
    cfg->LoadFile(TIXML_ENCODING_LEGACY);
    if(XmlError(cfg, sFile, buf)){
       delete cfg;
       return Assert_x(false,"Файл типов модулей", QString::fromLocal8Bit(buf));
    }
    cfg_root = cfg->RootElement();
    if(!Assert_x(cfg_root,"Файл типов модулей", "Не найден корневой элемент!")){
       delete cfg;
       return false;
    }
    e = cfg_root->FirstChildElement("Modules");
    if(!Assert_x(e,"Файл типов модулей", "Отсутствует описание модулей!")){
       return false;
    }
    for( n = e->FirstChildElement("Module"); n; n = n->NextSiblingElement() )
    {
        Modules modules;
        if(Assert_x(n->Attribute("Type",&i), "Файл типов модулей", "Указанный модуль без типа(Type)")){
           modules.setType(i);
        }
        const char* s = n->Attribute("Name");
        if(Assert_x(s, "Файл типов модулей", "Указанный модуль без имени(Name)")){

           modules.setName(QString::fromLocal8Bit(s));
        }
        s = n->Attribute("Color");
        if(Assert_x(s, "Файл типов модулей", "Указанный модуль без цвета(Color)")){
           if(QString(s).count(QRegExp("[abcdef]"))>0 || QString(s).length()<7){
              uint i = static_cast<uint>(strtol(s, nullptr, 16));
              modules.setColor(i);
           }else{
              modules.setColor(QString::fromLocal8Bit(s).toUInt());
           }
        }

        listTypeModules->append(modules);
    }
    delete cfg;
    return true;
}

/**
 * @brief MainWindow::fillList
 * @param ba
 */
void MainWindow::fillList(QByteArray ba)
{
    bufBytesRead->append(ba);
}

//Загрузка найденных модулей в таблицу//
/**
 * @brief MainWindow::loadModulesToModel
 */
void MainWindow::loadModulesToModel()
{
    listModul->clearModel();
    if(!bufBytesRead->isEmpty()){
        foreach (QByteArray var, *bufBytesRead){
            DataModules data;
            QString s;
            char buff[9];
            data.setNumLine(s.number(var[0]));
            data.setNumInternal(s.number(2-((static_cast<quint8>(var[2]) & 0x20)>>5)));
            data.setAddrDecMod(s.number(var[1]));
            sprintf(buff,"%02x",static_cast<quint8>(var[1]));
            data.setAddrHexMod("0x"+QString(buff));
            QString key = getKeyAdcPlo(data.getAddrHexMod(),s.number((2-((static_cast<quint8>(var[2]) & 0x20)>>5))-1));
            data.setAddrPLO(mapAdcPloAddr->value(key).getPloAddr());
            data.setTypeModule((static_cast<quint8>(var[4]) >> 4));
            s = getTypeModule(data.getTypeModule());
            data.setTypeMod(s);
            data.setStatusInver(s.number((static_cast<quint8>(var[2]) & 0x40)>>6));
            data.setSetAver(s.number(static_cast<quint8>((1<<static_cast<quint8>(var[5])))));
            sprintf(buff,"%02u.%02u",(static_cast<quint8>(var[3])>>4),(static_cast<quint8>(var[3])&0x0F));
            data.setVersionMod(QString(buff));
            sprintf(buff,"%02X%02X%02X%02X",static_cast<quint8>(var[6]),static_cast<quint8>(var[7]),static_cast<quint8>(var[8]),static_cast<quint8>(var[9]));
            data.setIdMod(QString(buff));
            data.setReservMod(s.number(static_cast<quint8>(((static_cast<quint8>(var[2]) & 0x80)>>7))));
            uint color = getColorModule(data.getTypeModule());
            data.setColorModuleRow(color);
            listModul->addCommand(data);
        }
    }
}

/**
 * @brief MainWindow::getTypeModule
 * @param type
 * @return
 */
QString MainWindow::getTypeModule(int type)
{
    QString retVal="UNK";

    foreach(Modules module, *listTypeModules){
       if(module.getType()==type){
          retVal = module.getName();
       }
    }
    return retVal;
}

/**
 * @brief MainWindow::getColorModule
 * @param type
 * @return
 */
uint MainWindow::getColorModule(int type)
{
    uint retVal=0;
    foreach(Modules module, *listTypeModules){
           if(module.getType()==type){
              retVal = module.getColor();
           }
    }
    return retVal;
}

/**
 * @brief MainWindow::getMapAdcs
 * @return
 */
QMap<QString, Adcs> *MainWindow::getMapAdcs() const
{
    return mapAdcs;
}

/**
 * @brief MainWindow::closeTest
 */
void MainWindow::closeTest()
{
    checkModules(techModule, 15 | (8<<4));
}

/**
 * @brief MainWindow::getMapAdcPloAddr
 * @return
 */
QMap<QString, AdcPloAddr> *MainWindow::getMapAdcPloAddr() const
{
    return mapAdcPloAddr;
}

/**
 * @brief MainWindow::loadAdcPloAddr
 * @param strFile
 */
void MainWindow::loadAdcPloAddr(QString strFile)
{
    char buf[160];
    QFile file(strFile);
    
    if(!file.exists(strFile)){
        QMessageBox::critical(this, "Сообщение", "Ошибка открытия файла modules.cfg ", QMessageBox::Ok);
        return;
    }

    file.setFileName(strFile);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
       while(file.readLine(buf, 160)!=-1){
             QString s(buf);
             if(s.count(';') != 9){
                 continue;
             }
             QStringList list = s.split(QRegExp("[=;]"));
             QString key = getKeyAdcPlo(list[2],list[3]);
             AdcPloAddr adcplo;
             adcplo.setPloAddr(list[6]);
             adcplo.setAdcAddr(static_cast<quint16>(list[7].toUShort()));
             mapAdcPloAddr->insert(key,adcplo);
       }
       file.close();
    }
}

/**
 * @brief MainWindow::loadAdcs
 * @param fileName
 */
void MainWindow::loadAdcs(QString fileName)
{
    char buf[1024];
    //QFile *file = new QFile();
    QFile file(fileName);

    if(!file.exists(fileName)){
        QMessageBox::critical(this, "Сообщение", "Ошибка открытия файла analog.cfg ", QMessageBox::Ok);
        return;
    }

    file.setFileName(fileName);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
       QByteArray *ba = new QByteArray(buf);
       while(file.readLine(buf, 1024)!=-1){
             //Изменим кодировку===============//
             QTextStream ts(ba);
             ts.setCodec("Windows-1251");
             QString s=ts.readAll();
             //================================//
             int cntSep = s.count('\t');
             QStringList list = s.split(QRegExp("[\t]"));
             if(list[0].contains(QRegExp("[0-9]")) && cntSep == 27){
                QString key = list[0];
                Adcs adcs;
                adcs.setName(list[1]);
                adcs.setShortName(list[2].trimmed());
                mapAdcs->insert(key,adcs);
             }
       }
       file.close();
       delete ba;
    }
}

/**
 * @brief MainWindow::getKeyAdcPlo
 * @param k1
 * @param k2
 * @return
 */
QString MainWindow::getKeyAdcPlo(QString k1, QString k2)
{
    return k1.right(2).trimmed()+k2.trimmed();

}

//Синхронизация////////////////////////////////////////////////
/**
 * @brief MainWindow::commandControlInd
 * @param cmd
 */
void MainWindow::commandControlInd(quint8 cmd)
{
    quint8 CMD = 0;
    switch(cmd){
       case 13:
       case 14:
       case 15:
          CMD = quint8(13 | (cmd << 4));
          break;
       case 2:
         CMD = cmd;
         break;
    }
    quint8 sz = kvkSIZ0[CMD] & 0x0F;
    for(int i = 0; i < LINE_COUNT; i++ ){//Поиск по всем линиям
        switch(i){
          case 0:
              emit sigWritePort0(ports.at(i), sz, 31, CMD);
              break;
          case 1:
              emit sigWritePort1(ports.at(i), sz, 31, CMD);
              break;
        }
    }
}

//===Slots====================================================================================================//
//Настройка модулей============================================================================//
/**
 * @brief MainWindow::settingModule
 */
void MainWindow::settingModule()
{
    DataModules data = getItemDataProxyModel(viewmodul->currentIndex());
    if(data.isData()){
       setModule = new SettingModule(&data, this);
       setModule->exec();
    }else{
        QMessageBox::information(this, "Сообщение", "Не выбрана строка в таблице!", QMessageBox::Ok);
    }

}

/**
 * @brief MainWindow::testRds
 */
void MainWindow::testRds()
{
    DataModules data = getItemDataProxyModel(viewmodul->currentIndex());

    if(!messageRDS(data)){return;}

     menuDisRDSNNS(false, true);

     if(!testRDS){
        testRDS = new TestRDS(this);
        testRDS->setWindowTitle("Тест РДС");
        testRDS->show();
        testRDS->raise();
        testRDS->activateWindow();
        testRDS->addRDS(data, RDS);
     }else{
         testRDS->addRDS(data, RDS);
         testRDS->show();
         testRDS->raise();
         testRDS->activateWindow();
     }
}

/**
 * @brief MainWindow::testRdsNnc
 */
void MainWindow::testRdsNnc()
{
    DataModules data = getItemDataProxyModel(viewmodul->currentIndex());

    if(!messageRDS(data)){return;}

    menuDisRDSNNS(true, false);

    if(!testRDSNNS){
       testRDSNNS = new TestRDS(this);
       testRDSNNS->setWindowTitle("Тест РДС + ННС");
       testRDSNNS->show();
       testRDSNNS->raise();
       testRDSNNS->activateWindow();
       testRDSNNS->addRDS(data, RDS_NNS);
    }else{
        testRDSNNS->addRDS(data, RDS_NNS);
        testRDSNNS->show();
        testRDSNNS->raise();
        testRDSNNS->activateWindow();
    }
}

/**
 * @brief MainWindow::menuDisRDSNNS
 * @param setrds
 * @param setrdsnns
 */
void MainWindow::menuDisRDSNNS(bool setrds, bool setrdsnns)
{
    actTestRds->setDisabled(setrds);
    actTestRdsHHC->setDisabled(setrdsnns);
}

/**
 * @brief MainWindow::messageRDS
 * @param data
 * @return
 */
bool MainWindow::messageRDS(DataModules data)
{
    if(!data.isData()){
        QMessageBox::information(this, "Сообщение", "Не выбрана строка в таблице!", QMessageBox::Ok);
        return false;
    }if(data.getTypeModule()!= 0){
        QMessageBox::information(this, "Сообщение", "Выберит модуль РДС!", QMessageBox::Ok);
        return false;
    }if(data.getAddrPLO().isEmpty()){
       QMessageBox::information(this, "Сообщение", "Неправильный адрес РДС!\nОтсутствует адрес ПЛО.", QMessageBox::Ok);
       return false;
    }

    return true;
}


//Поиск модулей================================================================================//
/**
 * @brief MainWindow::searchModule
 */
void MainWindow::searchModule()
{
    int i, step = 4;
    bufBytesRead->clear();
    activateProgress();
    int cmd = 14 | (1 << 4);
    int sz = kvkSIZ0[cmd] & 0x0F;   //extern const unsigned char kvkSIZ0[256];
    emit startProgress();
    //Поиск по всем линиям
    for(i = 0; i < LINE_COUNT; i++ ){
        connect(ports.at(i), SIGNAL(dataReady(QByteArray)), this, SLOT(fillList(QByteArray)));
        //Поиск по всем адресам
        for(int j = 0; j < 32; j++){
            switch(i){//Сигналы по линиям
            case 0:
                emit sigWritePort0(ports.at(i), sz, j, cmd);
                break;

            case 1:
                emit sigWritePort1(ports.at(i), sz, j, cmd);
                break;
            }
            switch(i){
                case 0:
                    emit sigReadPortAll0(ports.at(i), i+1, j, cmd);
                    break;
                case 1:
                    emit sigReadPortAll1(ports.at(i), i+1, j, cmd);
                    break;
            }
            emit changeProgress(++step);
        }
        disconnect(ports.at(i), SIGNAL(dataReady(QByteArray)), this, SLOT(fillList(QByteArray)));
    }
    emit stopProgress();
    loadModulesToModel();
    deactivateProgress();
}//============================================================================================//


//Включить индикацию==========//
/**
 * @brief MainWindow::indOn
 */
void MainWindow::indOn()
{
    commandControlInd(14);
}

//Отключить индикацию//
/**
 * @brief MainWindow::indOff
 */
void MainWindow::indOff()
{
    commandControlInd(15);
}

//Переменная индикацию//
/**
 * @brief MainWindow::indFlash
 */
void MainWindow::indFlash()
{
    commandControlInd(13);
}

/**
 * @brief MainWindow::checkTecModule
 * @param flag
 */
void MainWindow::checkTecModule(bool flag)
{
    if(!flag){
       DataModules zero;
       techModule=zero;//Очистка модуля//
       return;
    }
    //Запомним выбор ТМ//////////////////////////////////////////
    techModule = getItemDataProxyModel(viewmodul->currentIndex());
     if(!techModule.isData()){
         actCheckTecModule->setText(tr("Технологический модуль"));
         actCheckTecModule->setChecked(false);
         QMessageBox::information(this, "Сообщение", "Не выбрана строка в таблице!", QMessageBox::Ok);
     }else{
         if(actCheckTecModule->isChecked())
            actCheckTecModule->setText(tr("Технологический модуль ")+techModule.getTypeMod()+" "+techModule.getIdMod());
         else
           actCheckTecModule->setText(tr("Технологический модуль"));
     }
}

/**
 * @brief MainWindow::testModule
 */
void MainWindow::testModule()
{
    TestModule *testmod;
    //Запомним выбор OK/////////////////////////////////////////////////
    DataModules data = getItemDataProxyModel(viewmodul->currentIndex());
    //Запустим форму тестирования///////////////////////////////////////
    if(data.isData()){
       if(techModule.isData()){
          if((techModule.getAddrDecMod()==data.getAddrDecMod())
                  &&(techModule.getNumInternal()==data.getNumInternal())&&(techModule.getNumLine()==data.getNumLine())){
              QMessageBox::information(this, "Сообщение", "Выбранный модуль используется, как технологическй!", QMessageBox::Ok);
              return;
          }
       }else{
          QMessageBox::information(this, "Сообщение", "Не выбран технологическй модуль!", QMessageBox::Ok);
          return;
       }
       if(!checkModules(techModule, 14 | (1 << 4))){
          QMessageBox::information(this, "Сообщение от ТМ", "Нет ответа от технологического модуля!", QMessageBox::Ok);
          return;
       }
       if(!checkModules(data, 14 | (1 << 4))){
          QMessageBox::information(this, "Сообщение от ОК", "Нет ответа от объекта контроля!", QMessageBox::Ok);
          return;
       }
       testmod = new TestModule(&data, &techModule, this);
       if(!testmod->getSkipped()){
          testmod->close();//Закрытие и выход из формы тестирования//
       }
       else{
           if(setModeTested(data)){//ТМ и ОК в режим тестирования//
              testmod->exec();//Запуск формы тестирования//
           }else{
              testmod->close();//Закрытие и выход из формы тестирования//
           }
       }
    }else{
        QMessageBox::information(this, "Сообщение", "Не выбрана строка в таблице!", QMessageBox::Ok);
    }
}

//Перевод в режим тестирования ТМ и ОК///////////
/**
 * @brief MainWindow::setModeTested
 * @param data
 * @return
 */
bool MainWindow::setModeTested(DataModules data)
{
    bool retVal = true;

    if(!checkModules(data, 14 | (4 << 4))){
       QMessageBox::information(this, "Установка режима тестирования ОК", "Нет ответа от объекта контроля!", QMessageBox::Ok);
       retVal = false;
    }
    if(!checkModules(techModule, 14 | (0 << 4))){
       QMessageBox::information(this, "Установка режима тестирования ТМ", "Нет ответа от технологического модуля!", QMessageBox::Ok);
       retVal = false;
    }
    return retVal;
}


//Проверка ответа от ТМ и ОК/////////////////////////////////////
/**
 * @brief MainWindow::checkModules
 * @param module
 * @param command
 * @return
 */
bool MainWindow::checkModules(DataModules module, quint8 command)
{
    bool retVal = false;
    int numCom = 0;//Номер ком порта//
    int addrModule = 0;//Адрес модуля//
    quint8 cmd = command;//14 | (1 << 4);//Команда//

    numCom = module.getNumLine().toInt()-1;
    addrModule = module.getAddrDecMod().toInt();
    if(numCom == 0){
       emit sigCheckModules0(ports.at(numCom), cmd, addrModule, &retVal);
    }else if(numCom == 1){
       emit sigCheckModules1(ports.at(numCom), cmd, addrModule, &retVal);
    }

    return retVal;
}

//Константы скорости============================//
/**
 * @brief MainWindow::constSpeed
 */
void MainWindow::constSpeed()
{
    DataModules data = getItemDataProxyModel(viewmodul->currentIndex());
    if(data.isData()){
       rateConst = new RateConstants(&data, this);
       rateConst->exec();
    }else{
        QMessageBox::information(this, "Сообщение", "Не выбрана строка в таблице!", QMessageBox::Ok);
    }
}

//=== Калибрация МВАС ==========================//
/**
 * @brief MainWindow::calibrationAdcMbac
 */
void MainWindow::calibrationAdcMbac(){
    DataModules data = getItemDataProxyModel(viewmodul->currentIndex());
    if(data.isData()){
        calibrationMBAC = new CalibrationMBAC(&data, this);
        calibrationMBAC->exec();
        calibrationMBAC->setAttribute(Qt::WA_DeleteOnClose);
    }else{
        QMessageBox::information(this, "Сообщение", "Не выбрана строка в таблице!", QMessageBox::Ok);
    }

}

/**
 * @brief MainWindow::resetID
 */
void MainWindow::resetID()
{
    int numCom = 0;//Номер ком порта//
    int addrModule = 0;//Адрес модуля//
    quint8 cmd = 14 | (8 << 4);//Команда//

    DataModules data = getItemDataProxyModel(viewmodul->currentIndex());
    if(QMessageBox::warning(this,"Сообщение","Хотите установить случайный ID?",
                            QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Cancel)==QMessageBox::Cancel){
        return;
    }
    if(data.isData()){
       numCom = data.getNumLine().toInt()-1;
       addrModule = data.getAddrDecMod().toInt();
       if(numCom == 0){
          emit sigMakeFindItem0(ports.at(numCom), cmd, addrModule);
       }else if(numCom == 1){
          emit sigMakeFindItem1(ports.at(numCom), cmd, addrModule);
       }
    }else{
        QMessageBox::information(this, "Сообщение", "Не выбрана строка в таблице!", QMessageBox::Ok);
    }
}

/**
 * @brief MainWindow::resetKo
 */
void MainWindow::resetKo()
{
    int numCom = 0;//Номер ком порта//
    int addrModule = 0;//Адрес модуля//
    quint8 cmd = 14 | (6 << 4);//Команда//

    DataModules data = getItemDataProxyModel(viewmodul->currentIndex());
    if(QMessageBox::warning(this,"Сообщение","Хотите сбросить калибровочные коэффициенты?",
                            QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Cancel)==QMessageBox::Cancel){
        return;
    }
    if(data.isData()){
       numCom = data.getNumLine().toInt()-1;
       addrModule = data.getAddrDecMod().toInt();
       if(numCom == 0){
          emit sigMakeFindItem0(ports.at(numCom), cmd, addrModule);
       }else if(numCom == 1){
          emit sigMakeFindItem1(ports.at(numCom), cmd, addrModule);
       }
    }else{
        QMessageBox::information(this, "Сообщение", "Не выбрана строка в таблице!", QMessageBox::Ok);
    }
}


//Синхронизация====================================//
/**
 * @brief MainWindow::modeSync
 */
void MainWindow::modeSync()
{
     commandControlInd(2);

}

/**
 * @brief MainWindow::aBout
 */
void MainWindow::aBout()
{
  About *ab = new About(this);
  ab->exec();
}

//Калибровка=======================================//
/**
 * @brief MainWindow::makeCalibr
 */
void MainWindow::makeCalibr()
{
    DataModules data = getItemDataProxyModel(viewmodul->currentIndex());
    if(data.isData()){
        switch(data.getTypeModule()){
        case 9:
            calibrationMBAC = new CalibrationMBAC(&data, this);
            calibrationMBAC->exec();
            calibrationMBAC->setAttribute(Qt::WA_DeleteOnClose);
            break;
        default:
            calibr = new CalibrateDialog(&data, this);
            calibr->exec();
            calibr->setAttribute(Qt::WA_DeleteOnClose);
        }
    }else{
        QMessageBox::information(this, "Сообщение", "Не выбрана строка в таблице!", QMessageBox::Ok);
    }
}

//Обнаружить модуль===========================================================================================//
/**
 * @brief MainWindow::makeFindItem
 */
void MainWindow::makeFindItem()
{
    int numCom = 0;//Номер ком порта//
    int addrModule = 0;//Адрес модуля//
    quint8 cmd = 14|(13<<4);//Команда//

    DataModules data = getItemDataProxyModel(viewmodul->currentIndex());
    if(data.isData()){
       numCom = data.getNumLine().toInt()-1;
       addrModule = data.getAddrDecMod().toInt();
       if(numCom == 0){
          emit sigMakeFindItem0(ports.at(numCom), cmd, addrModule);
       }else if(numCom == 1){
          emit sigMakeFindItem1(ports.at(numCom), cmd, addrModule);
       }
    }else{
        QMessageBox::information(this, "Сообщение", "Не выбрана строка в таблице!", QMessageBox::Ok);
    }
}

//Уставки аналоговых сигналов=================================================================================//
/**
 * @brief MainWindow::makeSetPoint
 */
void MainWindow::makeSetPoint()
{
    DataModules data = getItemDataProxyModel(viewmodul->currentIndex());
    if(data.isData()){
       setanalog = new SettingsAnalog(&data, this);
       setanalog->exec();
    }else{
        QMessageBox::information(this, "Сообщение", "Не выбрана строка в таблице!", QMessageBox::Ok);
    }
}

/**
 * @brief MainWindow::printError
 * @param strMess
 */
void MainWindow::printError(QString strMess)
{
  QString strMsgSys = strMess;//Сообщение системное!//
  QString strErr = "Порт не найден.";
  //QMessageBox::critical(this, "Сообщение", strMsgSys, QMessageBox::Ok );
  QMessageBox::critical(this, "Сообщение", strErr, QMessageBox::Ok );
}

//Получение  выбранного QModelIndex ==========================================================================//
/**
 * @brief MainWindow::getItemDataProxyModel
 * @param modIdx
 * @return
 */
DataModules MainWindow::getItemDataProxyModel(QModelIndex modIdx)
{
    DataModules data;

    if(modIdx.isValid()){
      //Получим истинный индекс row выбраной строки в модели//
      int row = proxyModel->mapToSource(modIdx).row();
      data = listModul->getDataModules(row);
    }
    return data;
}

//+++++++Global++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
/**
 * @brief Assert_x
 * @param flag
 * @param title
 * @param message
 * @return
 */
bool Assert_x(bool flag, QString title, QString message)
{
   if(!flag){
      QMessageBox::critical(nullptr, title, message, QMessageBox::Ok);
      return false;
   }
   return true;
}

/**
 * @brief XmlError
 * @param cfg
 * @param filename
 * @param buff
 * @return
 */
bool XmlError(TiXmlDocument* cfg, const char *filename, char *buff)
{
  if (cfg->Error())
  {
    sprintf(buff,"XML: %s at %u row, %u col in file %s",cfg->ErrorDesc(),cfg->ErrorRow(),cfg->ErrorCol(),filename);
    return true;
  }
  return false;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
