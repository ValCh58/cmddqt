
#include <QMessageBox>
#include <QCloseEvent>
#include <QTextTable>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QDebug>

#include "ccmbb.h"
#include "testmodule.h"
#include "test.h"
#include "tinyxml/tinyxml.h"
#include "testtimer.h"


const int HIGHT_ITEM_LIST = 38;//Высота элемента меню списка//

TestModule::TestModule(DataModules *data, DataModules *datatech, QWidget *parent, Qt::WindowFlags f):QDialog(parent, f)
{
    mainWin = (MainWindow*)parent;
    sPorts = mainWin->ports;
    modul = data;//OK
    modulTech = datatech;//TM
    skipped = true;
    txtEdit = new QTextEdit("<span style=\"font-size:10pt;font-weight:600;color:green;\">Режим ожидания выбора теста!</span>");
    txtEdit->setAlignment(Qt::AlignCenter);
    txtEdit->setReadOnly(true);
    timer = new QTimer();
    timer->setInterval(1000);
    cntTime = 0;
    thrTest = 0;
    setModal(true);
    init();
    msgLayout->addWidget(txtEdit);
    setWindowIcon(QIcon(":/cmbbImages/setting_tools.png"));
    setWindowTitle("Тестирование модуля " + modul->getTypeMod()+" "+modul->getIdMod()+"//"+modul->getVersionMod());
    loadXMLTests("tests.xml");
    btnVisible(0);
    initMapIcon();
    initTestModules();
    cntOK = 0;
    cntInterrupt = 0;
    cntBad = 0;
    list->setFixedWidth(260);
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());

    connect(list, SIGNAL(clicked(QModelIndex)), this, SLOT(runTest(QModelIndex)));
    connect(btnStart, SIGNAL(clicked()), this, SLOT(startTest()));
    connect(btnClear, SIGNAL(clicked()), this, SLOT(clearPrepareTest()));
    connect(btnRep, SIGNAL(clicked()), this, SLOT(makeReport()));
    connect(timer, SIGNAL(timeout()), this, SLOT(testTime()));
    //Запись команды в ТМ===========================================================================================//
    connect(this, SIGNAL(sigStartTM0(SPort*,int,int,int)), sPorts.at(0), SLOT(writeStartTestTM(SPort*,int,int,int)));
    connect(this, SIGNAL(sigStartTM1(SPort*,int,int,int)), sPorts.at(1), SLOT(writeStartTestTM(SPort*,int,int,int)));
    //==============================================================================================================//
}

TestModule::~TestModule()
{
   if(timer){
        timer->stop();
        delete timer;
    }
    if(txtEdit){
        delete txtEdit;
    }
}

//Инициализация модулей ТМ и ОК, выбор допустимых тестов//
void TestModule::initTestModules()
{
      int i= 0;
      QString msg;
      selTests = new Tests();
      Tests::iterator p = tests->begin();
      while(p != tests->end()){
            Test *test = *p++;
            memset(test->Result,0,sizeof(test->Result));
            //Нужен ли этот тест для ОК?////////////
            if (test->MBB & (1 << (modul->getTypeModule())))
            {
               //Подходит ли ТМ к этому тесту?/////////
               if (test->MBB & (1 << (modulTech->getTypeModule() + 4))){
                   test->Order = i;//Переиндексация тестов////
                   selTests->insert(selTests->end(), test);//Доступные тесты для выполнения//
                   insertItemList(test);
                   i++;
               }
               else
               {
                   //Сообщить о том что выбор ТМ неверный///
                   skipped = false;
                   msg = test->Caption;
               }
            }
      }
      list->setFixedHeight(HIGHT_ITEM_LIST * ++i);
      if(!skipped){
         int ret = QMessageBox::warning(this, "Сообщение", "Cледующие обязательные тесты "
                                        "будут пропущены из за неправильного выбора ТМ: "+msg, QMessageBox::Ok | QMessageBox::Cancel);
         if(ret == QMessageBox::Ok)
            skipped = true;
      }
}

//Встака элемента выбора теста в список, перенос строк////////////
void TestModule::insertItemList(Test *t)
{
    QString s = t->Caption;
    int idx = s.indexOf(QChar(' '), 25);
    if(idx != -1){
       s.replace(idx, 1, QChar('\n'));
    }
    list->addItem(new QListWidgetItem(getIcon(t->ImageIndex), s));
}

//Управление портом для основного потока==============================//
void TestModule::controlPorts(bool flag)
{
    if(flag){
        QThread::msleep(500);
        sPorts.at(0)->connectPort();
        sPorts.at(1)->connectPort();
        QThread::msleep(100);
        startTestTM(0);
    }else{
        startTestTM(test->N);
        QThread::msleep(500);
        sPorts.at(0)->disconnectPort();
        sPorts.at(1)->disconnectPort();
    }
}//===================================================================//

//====================================================================//
//Исполнение тестов/////////////////////////////////////////////////////
//====================================================================//
//Счетчик времени выполнения==========================================//
void TestModule::testTime()
{
    cntTime++;
    uint min = cntTime/60;
    uint sec = cntTime - (min*60);
    QString dotblack("<span style=\"color:black;\">:</span>");
    QString dotred("<span style=\"color:red;\">:</span>");
    lcdNum->setText(QString::number(min).rightJustified(2, '0')+
                    (cntTime%2!=0?dotblack:dotred)
                    +QString::number(sec).rightJustified(2, '0'));
}

//Команда в ТМ=======================================================//
void TestModule::startTestTM(int cmdN)
{
    int numCom = 0;//Номер ком порта//
    int addrModule = 0;//Адрес модуля//
    quint8 cmd = (15 | (3 << 4));//Команда//

    numCom = modulTech->getNumLine().toInt()-1;
    addrModule = modulTech->getAddrDecMod().toInt();
    if(numCom == 0){
       emit sigStartTM0(sPorts.at(numCom), cmd, addrModule, cmdN);
    }else if(numCom == 1){
       emit sigStartTM1(sPorts.at(numCom), cmd, addrModule, cmdN);
    }
}//==================================================================//


//Обработка результатов теста===================================================================//
void TestModule::calcRetInfo(bool complit, bool user_actions, bool error_cycle, QByteArray info)
{
    QString msg;
    cntOK++;
    if(complit){
       retInfo = new QByteArray(info);
       QString s1 = printResultTestTimer(retInfo);//Результирующая строка//
       printColorToTextEdit(s1, QBrush(Qt::black), 10, "Areal");
       showResultCnt(cntOK, cntInterrupt);
       msg = "Тест окончен!";
    }else if(!user_actions && !error_cycle){
       cntInterrupt++;
       showResultCnt(cntOK, cntInterrupt);
       msg = "Тест прерван пользователем!";
    }else if(error_cycle){
        cntBad++;
        showResultCnt(cntOK, cntInterrupt);
        msg = "Ошибка при опросе модуля!";
    }
    test->Result[7] = 0;
    test->Result[8] = 0;
    test->Result[9] = 0;
    test->Result[10] = 0;
    test->Result[0] = cntOK;
    test->Result[1] = cntBad;
    test->Result[2] = cntInterrupt;
    makeResult();
    printColorToTextEdit(msg, complit?QBrush(Qt::blue):QBrush(Qt::red), 10, "Areal");
}//============================================================================================//

//Обработка результатов теста==================================================================//
void TestModule::calcRetInfo(bool error_cycle, int info)
{
    QString msg;
    if(!testModule->getUser_terminate()){
       test->Result[2]++;
       msg = "Тест прерван!";
    }else if(info==0 && !error_cycle){
       msg = "Тест закончен!";
    }else{
       msg = "Ошибка при опросе модуля!";
    }

    test->Result[0] += txTries->text().toInt();

    if(info!=0){
       test->Result[info] ++;
       cntBad++;
    }
    lbTries->setText("попыток в текущем тесте:");
    lbTries->setFrameStyle(QFrame::NoFrame);
    test->Result[1] = cntBad;
    makeResult();
    showResultCnt(test->Result[0], test->Result[1]);
    printColorToTextEdit(msg, error_cycle?QBrush(Qt::red):QBrush(Qt::blue), 10, "Areal");
}//============================================================================================//

//Счетчик попыток==============================================================================//
void TestModule::currSetCnt(int cnt){
    if(cnt >0 && cnt<3){
        lbTries->setText("попыток в текущей операции:");
        lbTries->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    }
    txTries->setText(QString::number(cnt));
    txFails->setText(QString::number(cntBad));
}//============================================================================================//

//Печать результатов теста таймера=============================================================//
QString TestModule::printResultTestTimer(QByteArray *bArr)
{
    char buff[180];
    unsigned char res[4];

    for(int i = 0; i < 4; i++){
        res[i] = abs((signed char)((quint8)bArr->at(i+2)));
    }

    chekResult(res[0] <= 200, 7, test);
    chekResult(res[1] <= 200, 8, test);
    chekResult(res[2] <= 200, 9, test);
    chekResult(res[3] <= 200, 10, test);
    sprintf(buff,"Отклонения таймеров, мс. таймер 0: %u; таймер 1: %u; таймер 2: %u; таймер 3: %u",res[0], res[1], res[3], res[4]);
    return QString(buff);
}//=====================================================================================================================================//

void TestModule::chekResult(bool val, int code, Test *test)
{
     if(!val){
         test->Result[code]++;
         cntBad++;
     }
}

//Заполнение таблицы результатами теста===========================================================================//
void TestModule::makeResult()
{
     int num = 1;
     insertTable(test->msgs->size()+1);

     //Результат измерений
     Msgs::iterator p = test->msgs->begin();
     while(p != test->msgs->end()){
       QTextCharFormat formatColor;
       if(test->Result[p.key()] > 0){
           formatColor.setForeground(QBrush(Qt::red));
       }else{
           formatColor.setForeground(QBrush(Qt::darkBlue));
       }

       table->cellAt(num, 0).firstCursorPosition().insertText(p.value(), formatColor);
       table->cellAt(num, 1).firstCursorPosition().insertText(QString::number(test->Result[p.key()]), formatColor);
       p++; num++;
     }
}//================================================================================================================//


//Таблица результатов теста========================================================================================//
void TestModule::insertTable(int row)
{
    QTextTableFormat format=getTableFormat(QColor("#e0e0e0"), Qt::AlignCenter);;
    QVector<QTextLength> constraints;//Kоллекция для задания размеров столбцов таблицы//
    constraints << QTextLength(QTextLength::PercentageLength, 90);//Ширина колонки//
    constraints << QTextLength(QTextLength::PercentageLength, 10);//Ширина колонки//
    format.setColumnWidthConstraints(constraints);//Set width columns//
    QTextCursor cursor(txtEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat formatHeader=getCharFormat("Courier", QFont::Bold, 11, QBrush(Qt::black), QBrush(QColor("#e0e0e0")));
    cursor.insertBlock();
    cursor.insertBlock();
    table =  cursor.insertTable(row, 2, format);
    table->mergeCells(0, 0, 1, 2);//Объеденим два столбца для заголовка//
    setItemCell(table, 0, 0, "Результаты тестирования", formatHeader, Qt::AlignCenter);
}//===============================================================================================================//



//Построение таблицы дискретных сигналов==========================================================================//
void TestModule::insertTableDiscret(int col)
{
    QTextTableFormat format=getTableFormat(QColor("#909090"), Qt::AlignCenter);
    QVector<QTextLength> constraints;//Kоллекция для задания размеров столбцов таблицы//
    constraints << QTextLength(QTextLength::PercentageLength, 7);
    for(int i = 0; i < col; i++){constraints << QTextLength(QTextLength::PercentageLength, col==16?5:3);}
    format.setColumnWidthConstraints(constraints);//Set width columns//
    QTextCursor cursor(txtEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat formatHeader=getCharFormat("MS Sans Serif", QFont::Bold, 10, QBrush(Qt::darkBlue), QBrush(QColor("#909090")));
    tableDiscret =  cursor.insertTable(4, col+1, format);
    tableDiscret->cellAt(0,0).setFormat(formatHeader);
    tableDiscret->mergeCells(0, 0, 1, col+1);//Объеденим столбцы для заголовка//
    setItemCell(tableDiscret, 0, 0, "Состояние дискретных сигналов ТМ и ОК", formatHeader, Qt::AlignCenter);
    setItemCell(tableDiscret, 1, 0, "TM",   formatHeader);
    setItemCell(tableDiscret, 2, 0, "OK",   formatHeader);
    setItemCell(tableDiscret, 3, 0, "Вход", formatHeader);
}//=============================================================================================================//

//Задание формата для таблицы===================================================================================//
QTextTableFormat TestModule::getTableFormat(QColor color, Qt::Alignment align)
{
    QTextTableFormat format;
    format.setCellPadding(3);
    format.setCellSpacing(0);
    format.setBackground(color);
    format.setAlignment(align);
    return format;
}

//Задание формата для символов==================================================================================================//
QTextCharFormat TestModule::getCharFormat(QString nameFont, QFont::Weight weight, int sizeFont, QBrush foreGrnd, QBrush backGrnd)
{
    QTextCharFormat formatHeader;
    formatHeader.setFontFamily(nameFont);
    formatHeader.setFontWeight(weight);
    formatHeader.setFontPointSize(sizeFont);
    formatHeader.setForeground(foreGrnd);
    formatHeader.setBackground(backGrnd);
    return formatHeader;
}


//Построение таблицы FAIN/АЦП===================================================================================//
void TestModule::insertTableACP(int AcpType, int ch)
{
    QTextTableFormat format = getTableFormat(QColor("#909090"), Qt::AlignCenter);
    QVector<QTextLength> constraints;//Kоллекция для задания размеров столбцов таблицы//
    for(int i = 0; i < 11; i++){constraints << QTextLength(QTextLength::PercentageLength, 9);}
    format.setColumnWidthConstraints(constraints);//Set width columns//
    QTextCursor cursor(txtEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat formatHeader=getCharFormat("MS Sans Serif", QFont::Bold, 9, QBrush(Qt::darkBlue), QBrush(QColor("#909090")));
    QTextCharFormat frm=getCharFormat("MS Sans Serif", QFont::Normal, 8, QBrush(Qt::black), QBrush(QColor("#909090")));
    //Построение и вставка таблицы/////////////////////
    if(AcpType == fainReal){
       tableFain =  cursor.insertTable(4, 11, format);
       tableFain->cellAt(0,0).setFormat(formatHeader);
       tableFain->mergeCells(0, 0, 1, 11);//Объеденим столбцы для заголовка//
       tableFain->mergeCells(1, 0, 2, 1);
       for(int i=1;i<=9;i+=2){tableFain->mergeCells(1, i, 1, 2);}
       QString Hg[]={"0","800","1600","2400","3200"};
       QString volt[]={"0,032 В","0,1 В"};
       setItemCell(tableFain, 0, 0, test->Caption, formatHeader, Qt::AlignCenter);
       setItemCell(tableFain, 3, 0, (ch==0?"FAIN1":"FAIN2"),   formatHeader);
       for(int i=1,j=0;i<=9;i+=2,j++){setItemCell(tableFain, 1, i, Hg[j]+" Гц", formatHeader, Qt::AlignCenter);}
       for(int i=1;i<=10;i++){setItemCell(tableFain, 2, i, i%2==0?volt[1]:volt[0], frm, Qt::AlignCenter);}
       frm=getCharFormat("MS Sans Serif", QFont::Normal, 8, QBrush(Qt::darkGreen), QBrush(QColor("#909090")));
       int idx=1;foreach(QTextTable *t,listTxtTblAcp){setItemCell(tableFain,3,idx++,t->cellAt(1,1).firstCursorPosition().block().text(),frm,Qt::AlignCenter);}
    }else if(AcpType == acpReal){
       tableFain =  cursor.insertTable(3, 5, format);
       tableFain->cellAt(0,0).setFormat(formatHeader);
       tableFain->mergeCells(0, 0, 1, 5);
       QString Hg[]={"7.2","10.4","13.6","16.8"};
       setItemCell(tableFain, 0, 0, test->Caption, formatHeader, Qt::AlignCenter);
       setItemCell(tableFain, 2, 0, (ch==0?"ACP1":"ACP2"),   formatHeader);
       for(int i=1;i<=4;i++){
           setItemCell(tableFain, 1, i, Hg[i-1]+" Ma", formatHeader, Qt::AlignCenter);
       }
       frm=getCharFormat("MS Sans Serif", QFont::Normal, 8, QBrush(Qt::darkGreen), QBrush(QColor("#909090")));
       int idx=1;
       foreach(QTextTable *t,listTxtTblAcp){
           setItemCell(tableFain,2,idx++,t->cellAt(1,1).firstCursorPosition().block().text(),frm,Qt::AlignCenter);
       }
    }else if(AcpType == acpCorrect){
        tableFain =  cursor.insertTable(3, 3, format);
        tableFain->cellAt(0,0).setFormat(formatHeader);
        tableFain->mergeCells(0, 0, 1, 3);
        QString Hg[]={"4","20"};
        setItemCell(tableFain, 0, 0, test->Caption, formatHeader, Qt::AlignCenter);
        setItemCell(tableFain, 2, 0, (ch==0?"ACP1":"ACP2"),   formatHeader);
        for(int i=1;i<=2;i++){
            setItemCell(tableFain, 1, i, Hg[i-1]+" Ma", formatHeader, Qt::AlignCenter);
        }
        frm=getCharFormat("MS Sans Serif", QFont::Normal, 8, QBrush(Qt::darkGreen), QBrush(QColor("#909090")));
        int idx=1;
        foreach(QTextTable *t,listTxtTblAcp){
            setItemCell(tableFain,2,idx++,t->cellAt(1,1).firstCursorPosition().block().text(),frm,Qt::AlignCenter);
        }
    }
}//=============================================================================================================//

//Таблица текущих значений измерений============================================================================//
void TestModule::makeTextTableAcp(QString header, QString name)
{
    QTextTableFormat format = getTableFormat(QColor("#909090"), Qt::AlignCenter);
    QTextCharFormat formatHeader=getCharFormat("MS Sans Serif", QFont::Bold, 9, QBrush(Qt::darkBlue), QBrush(QColor("#909090")));
    QVector<QTextLength> constraints;
    constraints << QTextLength(QTextLength::PercentageLength, 18);
    constraints << QTextLength(QTextLength::PercentageLength, 30);
    format.setColumnWidthConstraints(constraints);
    QTextCursor cursor(txtEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(2, 2, format);
    table->mergeCells(0, 0, 1,2);
    setItemCell(table, 0, 0, header, formatHeader, Qt::AlignCenter);
    setItemCell(table, 1, 0, name,   formatHeader);
    printColorToTextEdit(".......................................................................................................", QBrush(Qt::darkYellow), 11, "Areal");
    listTxtTblAcp.append(table);
}

void TestModule::getDataFromFAIN(QString color, QString val, int idxTbl)
{
    QTextCharFormat fcolor;
    if(color.indexOf("#00FF00")!=-1)
       fcolor = getCharFormat("MS Sans Serif", QFont::Bold, 9, QBrush(Qt::darkGreen), QBrush(QColor("#909090")));
    else
       fcolor = getCharFormat("MS Sans Serif", QFont::Bold, 9, QBrush(Qt::darkRed), QBrush(QColor("#909090")));

    setItemCell(listTxtTblAcp.at(idxTbl), 1, 1, val, fcolor, Qt::AlignCenter);
}

void TestModule::makeReport()
{
    /*QFile file("out.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);//поток для записи текста
    out << txtEdit->toHtml();
    file.close();*/

    QString fileName = QFileDialog::getSaveFileName(this, tr("Файл ODF"),"",tr("Файлы ODF (*.odf)"));
    if (fileName.isEmpty()){return;}

     QTextDocumentWriter writer(fileName);
     bool success;
     success = writer.write(txtEdit->document());
     if (success)
     {
         QMessageBox::information(this, "Экспорт отчета", "Экспорт прошел успешно");
     }
     else
     {
         QMessageBox::critical(this, "Экспорт отчета", "При сохранении файла произошла ошибка");
     }
}

//Заполнение таблицы при создании===============================================================================//
void TestModule::setSignals(ulong bufTm, ulong bufOk, int columns)
{
    QTextCharFormat fGreen;
    fGreen.setForeground(QBrush(Qt::darkGreen));

    for(int i=0; i<columns; i++){
        setSignal(bufTm, 1, i+1);
        setSignal(bufOk, 2, i+1);
        setItemCell(tableDiscret, 3, i+1, QString::number(i+1), fGreen, Qt::AlignCenter);
    }
}//============================================================================================================//

//Заполнение ячейки таблицы ===================================================================================//
void TestModule::setItemCell(QTextTable *tTable, int row, int col, QString s, QTextCharFormat frm)
{
    QTextCursor cursor = tTable->cellAt(row,col).firstCursorPosition();
    cursor.movePosition( QTextCursor::EndOfWord, QTextCursor::KeepAnchor );
    cursor.removeSelectedText();
    tTable->cellAt(row,col).firstCursorPosition().insertText(s, frm);
}

void TestModule::setItemCell(QTextTable *tTable, int row, int col, QString s, QTextCharFormat frm, Qt::Alignment alignment)
{
    QTextCursor cursor = tTable->cellAt(row,col).firstCursorPosition();
    QTextBlockFormat blockFormat = cursor.blockFormat();
    Qt::Alignment vertAlign = blockFormat.alignment() & Qt::AlignVertical_Mask;
    Qt::Alignment horzAlign = alignment & Qt::AlignHorizontal_Mask;
    Qt::Alignment combAlign = horzAlign | vertAlign;
    blockFormat.setAlignment(combAlign);
    cursor.setBlockFormat(blockFormat);
    cursor.movePosition( QTextCursor::EndOfLine, QTextCursor::KeepAnchor );
    cursor.removeSelectedText();
    tTable->cellAt(row,col).firstCursorPosition().insertText(s, frm);
}//==========================================================================================================//

//Возврат цвета символа из ячейки============================================================================//
QString TestModule::getColorName(QTextTable *tTable, int row, int col)
{
   return tTable->cellAt(row,col).firstCursorPosition().charFormat().foreground().color().name();
}//==========================================================================================================//

//Заполнение ячеек таблицы дискретов полученными сигналами===================================================//
void TestModule::checkSignals(ulong buf1, ulong buf2, int index)
{
    QTextCharFormat fRed;
    fRed.setForeground(QBrush(Qt::red));
    QTextCharFormat fGreen;
    fGreen.setForeground(QBrush(Qt::darkGreen));
    QTextCharFormat fYellow;
    fYellow.setForeground(QBrush(Qt::yellow));

    setSignal(buf1, 1, index);
    setSignal(buf2, 2, index);
    int currIdx = (index==1?tableDiscret->columns()-1:index-1);

    if(getColorName(tableDiscret, 3, currIdx).indexOf("#ff0000")==-1)
       setItemCell(tableDiscret, 3, currIdx, QString::number(currIdx), fGreen);
    if(getColorName(tableDiscret, 3, index).indexOf("#ff0000")==-1)
       setItemCell(tableDiscret, 3, index, QString::number(index), fYellow);
    if((((buf1 ^ buf2) >> (index-1)) & 1) ^ false){
        setItemCell(tableDiscret, 3, index, QString::number(index), fRed);
    }

    test->discr |= (buf1 ^ buf2);
    repaint();
}//========================================================================================================//

//Запись ошибок дискретов==================================================================================//
void TestModule::discrError()
{
    if(test->discr > 0){
        test->Result[BAD_SIGNAL]++;
        cntBad++;
        test->discr = 0;
    }
}//========================================================================================================//


void TestModule::resultTest()
{
   makeResult();
   printColorToTextEdit("Тест окончен пользователем.", QBrush(Qt::blue), 10, "Areal");
}

//Установка значения сигнала в ячейку таблицы=============================================================//
void TestModule::setSignal(ulong buf, int row, int idx)
{
    QTextCharFormat fgreen;
    fgreen.setForeground(QBrush(Qt::darkGreen));
    QTextCharFormat fblack;
    fblack.setForeground(QBrush(Qt::black));
    int maxCol = tableDiscret->columns()-1;

    if(idx>1){
       setItemCell(tableDiscret, row, idx-1, "0", fblack, Qt::AlignCenter);
    }
    if(idx==1){
       setItemCell(tableDiscret, row, maxCol, "0", fblack, Qt::AlignCenter);
    }

    if((buf & (1 << (idx-1)))){
        setItemCell(tableDiscret, row, idx, "1", fgreen, Qt::AlignCenter);
    }else{
        setItemCell(tableDiscret, row, idx, "0", fblack, Qt::AlignCenter);
    }
}//=====================================================================================================//

void TestModule::makeTableDiscret(int col)
{
     insertTableDiscret(col);
}//=====================================================================================================//

//Остановим поток правильно========================================================================//
bool TestModule::stopThread()
{
    bool retVal = true;
    int del = 0;

    if(thrTest->isRunning()){
       thrTest->exit();
    }
    while(!thrTest->wait(100)){
        if(del++ > 100){
            retVal = false;
            break;
        }
    }
    //qDebug()<<"thrTest->wait del:"<<del;
    return retVal;
}//===============================================================================================//


void TestModule::stopWorker()
{
   //Принудительная остановка потока выполнения теста===================//
   if(testModule->getSelf_terminate()||testModule->getUser_terminate()){
       testModule->setUser_terminate(false);//Поток прервал пользователь//
   //Завершение потока при оконченном тесте/////////
      if(stopThread()){
          btnVisible(test->disableStop);
          disconnect(btnStop, SIGNAL(clicked()), this, SLOT(stopWorker()));
          disconnect(thrTest, &QThread::finished, testModule, &QObject::deleteLater);
          timer->stop();
          list->setDisabled(false);
          controlPorts(CONNECT_PORT);//Подключим порты основного потока//
      }
   }
}

void TestModule::stop_worker()
{
    if(testModule->getUser_ready()&&testModule->getUser_terminate()){
       testModule->setUser_ready(false);
       testModule->setUser_terminate(false);
       QThread::msleep(500);
       if(stopThread()){
           btnVisible(test->disableStop);
           disconnect(btnStop, SIGNAL(clicked()), this, SLOT(stopWorker()));
           disconnect(thrTest, &QThread::finished, testModule, &QObject::deleteLater);
           timer->stop();
           list->setDisabled(false);
           controlPorts(CONNECT_PORT);//Подключим порты основного потока//
       }
    }
}//==================================================================================//

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Проведение теста таймеров ОК=========================================================================================================//
void TestModule::makeFormTestTimer()
{
    controlPorts(DIS_CONNECT_PORT);//Отключим порты основного потока//
    btnVisible(test->disableStart);//Отобразим кнопки на панели как надо//
    //-----------------------------------------------------------
    thrTest = new QThread();
    testModule = new TestTimer(mainWin->namePort,0,modulTech);
    testModule->moveToThread(thrTest);//Поместим объект в поток//
    connect(thrTest, &QThread::finished, testModule, &QObject::deleteLater);//Завершить поток и выйти//
    connect(btnStop, SIGNAL(clicked()), this, SLOT(stopWorker()));//Принудительно остановить поток до окончания теста//
    connect(this, SIGNAL(sigRunTest()), testModule, SLOT(startTest()));//Запустить процедуру в объекте потока для тестирования//
    connect(testModule, SIGNAL(sigStopTest()), this, SLOT(stopWorker()));//Нормальное завершение теста//
    connect(testModule, SIGNAL(sigCalcRetInfo(bool, bool, bool, QByteArray)), this, SLOT(calcRetInfo(bool, bool, bool, QByteArray)));//;
    thrTest->start();//Сtартуем поток//
    emit sigRunTest();//Сигнал на запуск теста//
}

//Проверка свето диодов//////////////////////////////////////////////////////////////////////////////
void TestModule::makeFormTestLamp()
{
    {//Отключим порты///////////////
     sPorts.at(0)->disconnectPort();
     sPorts.at(1)->disconnectPort();
     btnVisible(test->disableStart);
     QThread::msleep(100);
    }
    thrTest = new QThread();
    testModule = new TestLed(test->N, mainWin->namePort, 0, modulTech);
    testModule->moveToThread(thrTest);
    QThread::msleep(5);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    connect(thrTest, &QThread::finished, testModule, &QObject::deleteLater);
    connect(btnYes, SIGNAL(clicked()), this, SLOT(stopWorkerYes()));
    connect(btnNo, SIGNAL(clicked()), this, SLOT(stopWorkerNo()));
    connect(this, SIGNAL(sigRunTest()), testModule, SLOT(startTest()));
    connect(testModule, SIGNAL(sigCalcRetInfo()), this, SLOT(calcRetInfo()));
    connect(testModule, SIGNAL(sigStopTest()), this, SLOT(stopWorkerLed()));
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    thrTest->start();//Сtартуем поток//
    QThread::msleep(5);
    emit sigRunTest();//Сигнал на запуск теста//
    //-------------------------------------------------------------
}

void TestModule::stopWorkerYes()
{
    testModule->setUser_terminate(false);
    test->Result[0] = ++cntOK;
}

void TestModule::stopWorkerNo()
{
    testModule->setError_terminate(false);
    test->Result[1] = ++cntBad;
    test->Result[0] = ++cntOK;
}

void TestModule::calcRetInfo()
{
    showResultCnt(cntOK, cntBad);
    makeResult();
    printColorToTextEdit("Тест закончен пользователем!", QBrush(Qt::blue), 10, "Areal");

}

void TestModule::stopWorkerLed()
{
    if(stopThread()){
        disconnect(btnYes, SIGNAL(clicked()), this, SLOT(stopWorkerYes()));
        disconnect(btnNo, SIGNAL(clicked()), this, SLOT(stopWorkerNo()));
        disconnect(thrTest, &QThread::finished, testModule, &QObject::deleteLater);
        controlPorts(CONNECT_PORT);//Подключим порты основного потока//
        btnVisible(test->disableStop);
        timer->stop();
        list->setDisabled(false);
    }
}//=======================================================================================================//

void TestModule::makeFormTestUART1()
{
    controlPorts(DIS_CONNECT_PORT);//Отключим порты основного потока//
    btnVisible(test->disableStart);//Отобразим кнопки на панели как надо//
    makeTestUART0_1(new TestUART1(mainWin->namePort,modul,modulTech));
}

void TestModule::makeFormTestUART0()
{
    controlPorts(DIS_CONNECT_PORT);//Отключим порты основного потока//
    btnVisible(test->disableStart);//Отобразим кнопки на панели как надо//
    makeTestUART0_1(new TestUART0(mainWin->namePort,modul,modulTech));
}

void TestModule::makeFormTestKNO_AWO()//==================================================================//
{
    controlPorts(DIS_CONNECT_PORT);//Отключим порты основного потока//
    btnVisible(test->disableStart);//Отобразим кнопки на панели как надо//
    //--------------------------------------------------------------------
    thrTest = new QThread();
    testModule = new CheckKnoAwo(mainWin->namePort,0,modulTech);
    testModule->moveToThread(thrTest);//Поместим объект в поток//
    connect(thrTest, &QThread::finished, testModule, &QObject::deleteLater);//Завершить поток и выйти//
    connect(btnStop, SIGNAL(clicked()), this, SLOT(stopWorker()));//Принудительно остановить поток до окончания теста//
    connect(this, SIGNAL(sigRunTest(uint*)), testModule, SLOT(startTest(uint*)));//Запустить процедуру в объекте потока для тестирования//
    connect(testModule, SIGNAL(sigStopTest()), this, SLOT(stopWorker()));//Нормальное завершение теста//
    connect(testModule, SIGNAL(sigSetCnt(int)), this, SLOT(currSetCnt(int)));
    connect(testModule, SIGNAL(sigCalcRetInfo(bool, int)), this, SLOT(calcRetInfo(bool, int)));
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    thrTest->start();//Сtартуем поток//
    emit sigRunTest(test->Result);//Сигнал на запуск теста//
    //-------------------------------------------------------------------
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//НВС проверка узла дискретного ввода сигналов=============================================================//
void TestModule::makeFormTestDiscrHBC()
{
    controlPorts(DIS_CONNECT_PORT);//Отключим порты основного потока//
    btnVisible(test->disableStart);//Отобразим кнопки на панели как надо//
    //--------------------------------------------------------------------
    thrTest = new QThread();
    testModule = new TestDiscrNBC(mainWin->namePort,modul,modulTech);
    testModule->moveToThread(thrTest);//Поместим объект в поток//
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
    connect(thrTest, &QThread::finished, testModule, &QObject::deleteLater);//Завершить поток и выйти//
    connect(btnStop, SIGNAL(clicked()), this, SLOT(stop_worker()));//Принудительно остановить поток//
    connect(this, SIGNAL(sigRunTest()), testModule, SLOT(startTest()));//Запустить процедуру в объекте потока для тестирования//
    connect(testModule, SIGNAL(sigStopTest()), this, SLOT(stop_worker()));//Нормальное завершение теста//
    connect(testModule, SIGNAL(sigMakeTableDiscret(int)), this, SLOT(makeTableDiscret(int)));
    connect(testModule, SIGNAL(sigCalcRetInfo(bool, int)), this, SLOT(calcRetInfo(bool, int)));
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
    connect(testModule, SIGNAL(sigPrintMsg(QString, int)), this, SLOT(printMsgFromThrd(QString, int)));
    connect(testModule, SIGNAL(sigStateBtn(quint8)), this, SLOT(stateBtn(quint8)));
    connect(btnReady, SIGNAL(clicked()), this, SLOT(changeUserReady()));
    connect(testModule, SIGNAL(sigSetSignalDiscret(ulong,ulong,int)), this, SLOT(setSignals(ulong,ulong,int)));
    connect(testModule, SIGNAL(sigCheckSignals(ulong,ulong,int)), this, SLOT(checkSignals(ulong,ulong,int)));
    connect(testModule, SIGNAL(sigDiscrError()), this, SLOT(discrError()));
    connect(testModule, SIGNAL(sigSetCnt(int)), this, SLOT(currSetCnt(int)));
     //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
    thrTest->start();//Сtартуем поток//
    emit sigRunTest();//Сигнал на запуск теста//
    //-------------------------------------------------------------------
}


void TestModule::makeFormTestCalibACP1()
{
    listTxtTblAcp.clear();
    controlPorts(DIS_CONNECT_PORT);
    btnVisible(test->disableStart);
    makeTestAcp(new TestCalibrCh(0,mainWin->namePort,modul,modulTech));
}

void TestModule::makeFormTestCalibACP2()
{
    listTxtTblAcp.clear();
    controlPorts(DIS_CONNECT_PORT);
    btnVisible(test->disableStart);
    makeTestAcp(new TestCalibrCh(1,mainWin->namePort,modul,modulTech));
}

void TestModule::makeFormTestCheckACP1()
{
    listTxtTblAcp.clear();
    controlPorts(DIS_CONNECT_PORT);
    btnVisible(test->disableStart);
    makeTestAcp(new TestACP(0,mainWin->namePort,modul,modulTech));
}

void TestModule::makeFormTestCheckACP2()
{
    listTxtTblAcp.clear();
    controlPorts(DIS_CONNECT_PORT);
    btnVisible(test->disableStart);
    makeTestAcp(new TestACP(1,mainWin->namePort,modul,modulTech));
}

void TestModule::makeFormTestDiscrNNC()
{
    controlPorts(DIS_CONNECT_PORT);
    btnVisible(test->disableStart);
    makeTestNNC_KDC(new TestDiscrNNC(0x3C000000, 0x33000000, mainWin->namePort,modul,modulTech));
}

void TestModule::makeFormTestDiscrPDC()
{
    controlPorts(DIS_CONNECT_PORT);
    btnVisible(test->disableStart);
    makeTestNNC_KDC(new TestDiscrNNC(0x00400000, 0x00000000, mainWin->namePort,modul,modulTech));
}

void TestModule::makeFormTestDiscrKDC()
{
    controlPorts(DIS_CONNECT_PORT);
    btnVisible(test->disableStart);
    makeTestNNC_KDC(new TestDiscrNNC(0x00800000, 0x00400000, mainWin->namePort,modul,modulTech));
}

void TestModule::makeFormTestCheckFAIN1()
{
    listTxtTblAcp.clear();
    controlPorts(DIS_CONNECT_PORT);
    btnVisible(test->disableStart);
    makeTestAcp(new TestCheckFAIN1(0,mainWin->namePort,modul,modulTech));
}

void TestModule::makeFormTestCheckFAIN2()
{
    listTxtTblAcp.clear();
    controlPorts(DIS_CONNECT_PORT);
    btnVisible(test->disableStart);
    makeTestAcp(new TestCheckFAIN1(1,mainWin->namePort,modul,modulTech));
}

void TestModule::makeTestAcp(Worker *w)
{
    thrTest = new QThread();
    testModule = w;
    testModule->moveToThread(thrTest);//Поместим объект в поток//
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
    connect(thrTest, &QThread::finished, testModule, &QObject::deleteLater);//Завершить поток и выйти//
    connect(btnStop, SIGNAL(clicked()), this, SLOT(stop_worker()));//Принудительно остановить поток//
    connect(this, SIGNAL(sigRunTest()), testModule, SLOT(startTest()));//Запустить процедуру в объекте потока для тестирования//
    connect(testModule, SIGNAL(sigStopTest()), this, SLOT(stop_worker()));//Нормальное завершение теста//
    connect(testModule, SIGNAL(sigPrintMsg(QString, int)), this, SLOT(printMsgFromThrd(QString, int)));
    connect(testModule, SIGNAL(sigACPTable(int,int)), this, SLOT(insertTableACP(int,int)));
    connect(btnReady, SIGNAL(clicked()), this, SLOT(changeUserReady()));
    connect(testModule, SIGNAL(sigCalcRetInfo(bool, int)), this, SLOT(calcRetInfo(bool, int)));
    connect(testModule, SIGNAL(sigMakeTextTableAcp(QString, QString)),this, SLOT(makeTextTableAcp(QString, QString)));
    connect(testModule, SIGNAL(sigUpdateTableAcp(QString,QString,int)), this, SLOT(getDataFromFAIN(QString,QString,int)));
    connect(testModule, SIGNAL(sigSetCnt(int)), this, SLOT(currSetCnt(int)));
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
    thrTest->start();//Сtартуем поток//
    emit sigRunTest();//Сигнал на запуск теста//
}

void TestModule::makeTestNNC_KDC(Worker *w)
{
    thrTest = new QThread();
    testModule = w;//new TestDiscrNNC(0x00800000, 0x00400000, mainWin->namePort,modul,modulTech);
    testModule->moveToThread(thrTest);//Поместим объект в поток//
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
    connect(thrTest, &QThread::finished, testModule, &QObject::deleteLater);//Завершить поток и выйти//
    connect(btnStop, SIGNAL(clicked()), this, SLOT(stop_worker()));//Принудительно остановить поток//
    connect(this, SIGNAL(sigRunTest()), testModule, SLOT(startTest()));//Запустить процедуру в объекте потока для тестирования//
    connect(testModule, SIGNAL(sigStopTest()), this, SLOT(stop_worker()));//Нормальное завершение теста//
    connect(testModule, SIGNAL(sigMakeTableDiscret(int)), this, SLOT(makeTableDiscret(int)));
    connect(testModule, SIGNAL(sigCalcRetInfo(bool, int)), this, SLOT(calcRetInfo(bool, int)));
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
    connect(testModule, SIGNAL(sigPrintMsg(QString, int)), this, SLOT(printMsgFromThrd(QString, int)));
    connect(testModule, SIGNAL(sigSetSignalDiscret(ulong,ulong,int)), this, SLOT(setSignals(ulong,ulong,int)));
    connect(testModule, SIGNAL(sigCheckSignals(ulong,ulong,int)), this, SLOT(checkSignals(ulong,ulong,int)));
    connect(testModule, SIGNAL(sigSetCnt(int)), this, SLOT(currSetCnt(int)));
    thrTest->start();
    emit sigRunTest();
}

void TestModule::makeTestUART0_1(Worker *w)
{
    thrTest = new QThread();
    testModule = w;//new TestUART0(mainWin->namePort,modul,modulTech);
    testModule->moveToThread(thrTest);//Поместим объект в поток//
    connect(thrTest, &QThread::finished, testModule, &QObject::deleteLater);//Завершить поток и выйти//
    connect(btnStop, SIGNAL(clicked()), this, SLOT(stopWorker()));//Принудительно остановить поток до окончания теста//
    connect(this, SIGNAL(sigRunTest()), testModule, SLOT(startTest()));//Запустить процедуру в объекте потока для тестирования//
    connect(testModule, SIGNAL(sigStopTest()), this, SLOT(stopWorker()));//Нормальное завершение теста//
    connect(testModule, SIGNAL(sigSetCnt(int)), this, SLOT(currSetCnt(int)));
    connect(testModule, SIGNAL(sigCalcRetInfo(bool, int)), this, SLOT(calcRetInfo(bool, int)));
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    thrTest->start();//Сtартуем поток//
    QThread::msleep(25);
    emit sigRunTest();//Сигнал на запуск теста//
}

//=============================================================================================================================//

void TestModule::changeUserReady()
{
    testModule->setUser_ready(false);
}

void TestModule::printMsgFromThrd(QString msg, int color)
{
    QBrush br(Qt::SolidPattern);
    switch(color){
    case 1:
       br.setColor(Qt::darkYellow);
       break;
    case 2:
        br.setColor(Qt::darkGreen);
        break;
    case 3:
        br.setColor(Qt::darkRed);
        break;
    case 4:
        br.setColor(Qt::darkBlue);
        break;
    default:
        br.setColor(Qt::black);
    }
    printColorToTextEdit(msg, br, 11, "Areal");
}

void TestModule::stateBtn(quint8 flagDisable)
{
    btnVisible(flagDisable);//Отобразим кнопки на панели как надо//
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

//Подготовка теста к выполнению/////////////////////////////////////////////
void TestModule::prepareTest(QString caption, QString abstract)
{
    QTextCursor cursor(txtEdit->textCursor());
    cursor.movePosition(QTextCursor::Start);
    QTextCharFormat textFormat;
    textFormat.setFontWeight(QFont::Cursive);
    textFormat.setFontItalic(true);
    QTextCharFormat boldFormat;
    boldFormat.setFontWeight(QFont::Bold);
    cursor.insertText(caption, boldFormat);
    cursor.insertBlock();
    cursor.insertBlock();
    cursor.insertText(abstract, textFormat);
    cursor.insertBlock();
    cursor.insertText("Нажмите кнопку \"Старт\" для запуска теста.", boldFormat);
    cursor.insertBlock();
    cursor.insertBlock();
}

void TestModule::prnMessageTest(QString msg)
{
    QString str = "Тест запущен!";
    QTextCursor cursor(txtEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat formatColor;
    formatColor.setForeground(QBrush(Qt::blue));
    formatColor.setFont(QFont("Areal",10));
    cursor.insertText(str, formatColor);
    cursor.insertBlock();
    QTextCharFormat boldFormat;
    boldFormat.setFontWeight(QFont::Bold);
    cursor.insertText(msg, boldFormat);
    cursor.insertBlock();
    txtEdit->setTextCursor(cursor);
}





void TestModule::printColorToTextEdit(QString str, QBrush brush, int szFont, QString nmFont)
{
    QTextCursor cursor(txtEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat formatColor;
    formatColor.setForeground(brush);
    formatColor.setFont(QFont(nmFont,szFont));
    cursor.insertText(str, formatColor);
    cursor.insertBlock();
    txtEdit->setTextCursor(cursor);
}





//Close Event/////////////////////////////////
void TestModule::closeEvent(QCloseEvent *event)
{   //Возврат модулей в исходное состояние//
    if(thrTest && thrTest->isRunning()){
        QMessageBox::information(this, test->Caption, "Из режима тестирования можно выйти после окончания или прерывания теста!");
        event->ignore();
        return;
    }
    mainWin->closeTest();
    event->accept();
}

void TestModule::btnVisible(const quint8 mask) const
{
    //btnPrev->setDisabled(!(mask  & BTNPREV));
    //btnNext->setDisabled(!(mask  & BTNNEXT));
    btnStart->setDisabled(!(mask & BTNSTART));
    btnStop->setDisabled(!(mask  & BTNSTOP));
    btnReady->setDisabled(!(mask & BTNREADY));
    btnYes->setDisabled(!(mask   & BTNYES));
    btnNo->setDisabled(!(mask    & BTNNO));
    btnClear->setDisabled(!(mask & BTNCLEAR));
}

//Фабрика кнопок. Текст под иконкой//////////////////////////////////
QToolButton *TestModule::makeToolButton(QIcon ico, QString str) const
{
    QToolButton *btn = new QToolButton();
    btn->setIcon(ico);
    btn->setIconSize(QSize(24, 24));
    btn->setText(str);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setFixedSize(52, 52);
    btn->setCursor(QCursor(Qt::PointingHandCursor));
    return btn;
}

//Слои тестов///////////////////////////////////////////////////////////////
QHBoxLayout *TestModule::createBtnLayout()
{
    QHBoxLayout *hLayout = new QHBoxLayout();
    //btnPrev = makeToolButton(QIcon(":/cmbbImages/btnPrev.png"), "Назад");
    //hLayout->addWidget(btnPrev);
    //btnNext = makeToolButton(QIcon(":/cmbbImages/btnNext.png"), "Вперед");
    //hLayout->addWidget(btnNext);
    btnStart = makeToolButton(QIcon(":/cmbbImages/btnStart.png"), "Старт");
    hLayout->addWidget(btnStart);
    btnStop = makeToolButton(QIcon(":/cmbbImages/btnStop.png"), "Стоп");
    hLayout->addWidget(btnStop);
    btnReady = makeToolButton(QIcon(":/cmbbImages/btnReady.png"), "Готов");
    hLayout->addWidget(btnReady);
    btnYes = makeToolButton(QIcon(":/cmbbImages/btnYes.png"), "Да");
    hLayout->addWidget(btnYes);
    btnNo = makeToolButton(QIcon(":/cmbbImages/btnNo.png"), "Нет");
    hLayout->addWidget(btnNo);
    btnClear = makeToolButton(QIcon(":/cmbbImages/btnClear.png"), "Очистка");
    btnClear->setToolTip("Очистка результатов теста");
    hLayout->addWidget(btnClear);
    btnRep = makeToolButton(QIcon(":/cmbbImages/btnRep.png"), "Отчет");
    btnRep->setToolTip("Экспорт отчета в файл Open Office");
    hLayout->addWidget(btnRep);
    return hLayout;
}

QHBoxLayout *TestModule::createStatistLayout()
{
    QHBoxLayout *hLayout = new QHBoxLayout();
    lbTries = new QLabel("попыток в текущем тесте:");
    txTries = new QLineEdit("0");
    txTries->setAlignment(Qt::AlignRight);
    txTries->setFixedWidth(60);
    txTries->setReadOnly(true);
    lbFails = new QLabel(" всего неудачных:");
    txFails = new QLineEdit("0");
    txFails->setAlignment(Qt::AlignRight);
    txFails->setFixedWidth(60);
    txFails->setReadOnly(true);
    labNum = new QLabel("Время выполнения:");
    lcdNum = new QLabel("00:00");
    lcdNum->setFont(QFont("Helvetica", 10, QFont::DemiBold));
    hLayout->addSpacing(8);
    hLayout->addWidget(lbTries);
    hLayout->addWidget(txTries);
    hLayout->addSpacing(10);
    hLayout->addWidget(lbFails);
    hLayout->addWidget(txFails);
    hLayout->addSpacing(37);
    hLayout->addWidget(labNum);
    hLayout->addWidget(lcdNum);
    hLayout->addSpacing(3);
    return hLayout;
}

QVBoxLayout *TestModule::createMsgLayout()
{
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(txtEdit);
    msgLayout = vLayout;
    return vLayout;
}
//==========================================//

//Выбор теста из списка слева=============================================//
void TestModule::runTest(QModelIndex idx)
{
    static QModelIndex indOld;
    if(idx == indOld && cntTime > 0)
        return;

    if(cntTime > 0){
        QMessageBox::information(this, test->Caption,
          "Перед выбором из списка другого теста - нажмите кнопку \"Очистка\".\nПосле очистки результаты текущего теста будут удалены!");
        list->setCurrentIndex(indOld);
        return;
    }

    indOld = idx;//Сохраним старый индекс для возврата//
    int row = idx.row();
    test = getTestNumber(row);
    txtEdit->clear();
    prepareTest(test->Caption, test->Abstract);
    btnVisible(test->disableStop);
    cntOK = 0;
    cntInterrupt = 0;
    cntBad = 0;
    showResultCnt(cntOK, cntInterrupt);
}//=====================================================================//

//Отображение статистики по выполнению теста============================//
void TestModule::showResultCnt(int trise, int fails){
    txTries->setText(QString::number(trise));
    txFails->setText(QString::number(fails));
}//=====================================================================//

//Получим номер теста===================================================//
Test* TestModule::getTestNumber(int idx)
{
    Test *retVal = 0;

    for(int i=0; i<selTests->size(); i++){
        if(selTests->at(i)->Order == idx){
            retVal = selTests->at(i);
            break;
        }
    }
    return retVal;
}//====================================================================//

//Запуск таймера=======================================================//
void TestModule::timerStartForTest()
{
    timer->start();//Время исполнения теста//
    lcdNum->setText("00:00");
    cntTime = 0;
    list->setDisabled(true);
}//===================================================================//

//Старт теста=========================================================//
void TestModule::startTest()
{
    prnMessageTest(test->Message);
    timerStartForTest();

    switch(test->Nex){
    case 31: //Калибровка узла измерения тока №1, включим соседа//
        makeFormTestCalibACP1();
        break;
    case 32: //Калибровка узла измерения тока №2, включим соседа//
        makeFormTestCalibACP2();
        break;

    case 40: //Проверка узла измерения тока №1, включим соседа//
        makeFormTestCheckACP1();
        break;
    case 41: //Проверка узла измерения тока №2, включим соседа//
        makeFormTestCheckACP2();
        break;
    case 42: //Проверка узла измерения частоты №1, включим соседа//
        makeFormTestCheckFAIN1();
        break;
    case 43: //Проверка узла измерения частоты №2, включим соседа//
        makeFormTestCheckFAIN2();
        break;
    case 33: //КДС, сбросим все ошибки//
        makeFormTestDiscrKDC();
        break;
    case 34: //РДС, сбросим все ошибки//
        makeFormTestDiscrPDC();
        break;
    case 35: //ННС, сбросить все ошибки//Проверка узла ввода дискретных сигналов OK ННС
        makeFormTestDiscrNNC();
        break;
    case 36: //НВС, сбросим все ошибки//Проверка узла ввода дискретных сигналов OK НBС
        makeFormTestDiscrHBC();
        break;
    case 1: //Проверка таймеров//
        makeFormTestTimer();
        break;
    case 2: //Проверка сигналов KNO, AWO//
        makeFormTestKNO_AWO();
        break;
    case 5: //Проверка UART1//
        makeFormTestUART1();
        break;
    case 6: //Проверка UART0//
        makeFormTestUART0();
        break;
    case 3:
    case 4: //Проверка светодиодов//
        makeFormTestLamp();
        break;
    }
}//=======================================================================================//

//Очистка данных теста====================================================================//
void TestModule::clearPrepareTest()
{
    txtEdit->clear();
    prepareTest(test->Caption, test->Abstract);
    lcdNum->setText("00:00");
    cntTime = 0;
    cntBad = 0;
    cntInterrupt = 0;
    cntOK = 0;
    memset(test->Result, 0, sizeof(test->Result));
    test->discr = 0;
    showResultCnt(0, 0);
}//======================================================================================//

bool TestModule::getSkipped() const
{
    return skipped;
}

//Инициализация интерфейса формы////////////////
void TestModule::init()
{
    QHBoxLayout *formBox = new QHBoxLayout();
    formBox->addWidget(makeMenu());
    formBox->addWidget(makeFormTest(680));
    setLayout(formBox);
}

//Создание меню для выбора тестов///////////////////
QGroupBox *TestModule::makeMenu()
{
    QGroupBox *gBox = new QGroupBox("Выбор тестов");
    QVBoxLayout *menuBox = new QVBoxLayout();
    list = new QListWidget();
    list->setIconSize(QSize(36, 36));
    menuBox->addWidget(list);
    gBox->setLayout(menuBox);
    return gBox;
}

QGroupBox *TestModule::makeFormTest(int width)
{
    QGroupBox *gBoxForm = new QGroupBox("Окно тестирования модулей");
    gBoxForm->setFixedWidth(width);
    QVBoxLayout *main = new QVBoxLayout();
    main->addLayout(createBtnLayout());
    main->addLayout(createStatistLayout());
    main->addLayout(createMsgLayout());
    gBoxForm->setLayout(main);
    return gBoxForm;
}

//Загрузка ХМЛ файла описания тестов///////
void TestModule::loadXMLTests(QString file)
{
    TiXmlDocument* cfg;
    TiXmlElement* cfg_root;
    int i=0;
    TiXmlElement* e;
    TiXmlElement* n;
    char buf[256];

     QByteArray ba = file.toUtf8();//QString
     const char *sFile = ba.data();// в char*
     cfg = new TiXmlDocument(sFile/*"tests.xml"*/);
     cfg->LoadFile(TIXML_ENCODING_LEGACY);
     if(XmlError(cfg, sFile, buf)){
        delete cfg;
        Assert_x(false,"Файл типов модулей", QString::fromLocal8Bit(buf));
        return;
     }
     cfg_root = cfg->RootElement();
     if(!Assert_x(cfg_root, "Сообщение","CFG: Не найден корневой элемент!")){
        delete cfg;
        return;
     }
     //читаем описание тестов модулей
     i = 0;
     tests = new Tests();
     e = cfg_root->FirstChildElement("Tests");
     if(!Assert_x(e, "Сообщение", "CFG: Отсутствует описание тестов модулей!")){
         delete cfg;
         return;
     }
     for( n = e->FirstChildElement("Test"); n; n = n->NextSiblingElement() )
     {
       Test* tst = new Test(n);
       tst->Order = i;
       //Добавим в список тестов
       tests->insert(tests->end(), tst);
       i++;
     }
     delete cfg;
}

//Создание коллекции иконок для меню тестов////////////
void TestModule::initMapIcon()
{
    mapIcon.insert(0, QIcon(":/cmbbImages/conn1.png"));
    mapIcon.insert(1, QIcon(":/cmbbImages/mbb1.png"));
    mapIcon.insert(2, QIcon(":/cmbbImages/mbb2.png"));
    mapIcon.insert(3, QIcon(":/cmbbImages/port1.png"));
    mapIcon.insert(4, QIcon(":/cmbbImages/ram1.png"));
    mapIcon.insert(5, QIcon(":/cmbbImages/discr.png"));
    mapIcon.insert(6, QIcon(":/cmbbImages/chip.png"));
    mapIcon.insert(7, QIcon(":/cmbbImages/anal.png"));
    mapIcon.insert(8, QIcon(":/cmbbImages/svet1.png"));
    mapIcon.insert(9, QIcon(":/cmbbImages/svet2.png"));
}

//Получение иконки из коллекции//////
QIcon TestModule::getIcon(int numImg)
{
   return mapIcon.value(numImg);
}




