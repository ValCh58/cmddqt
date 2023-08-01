#include "testtimer.h"
#include <QDebug>


TestTimer::TestTimer(QList<QString> portname, DataModules *modok, DataModules *modtm, QObject *parent) : Worker(portname,modok, modtm, parent)
{
   self_terminate = false;
   user_terminate = true;
   error_terminate = false;
}

TestTimer::~TestTimer()
{

}

void TestTimer::startTest()
{
   int i = 0;
   //QSerialPort *port = listPorts.at(moduleTM->getNumLine().toInt()-1);
   //connectPort(port);

   QThread::msleep(10);

   while(!self_terminate && user_terminate && !error_terminate){//user_terminate=false нажата кнопка СТОП до завершения окончания цикла
       QThread::msleep(10);
       if(writeTModule(portTm, 15 | (5 << 4))){
           self_terminate = true;//Нормальное завершение цикла
       }
       i++;
       if(i>1000){
           error_terminate = true;//Произошло зависание цикла
           //emit sigStopTest();
       }
       //qDebug()<<"Count:"<<i;
   }
   emit sigCalcRetInfo(self_terminate, user_terminate, error_terminate, RX);//Сигнал на обработку информации//
   if(user_terminate){
      emit sigStopTest();
   }
}




bool TestTimer::writeTModule(QSerialPort *thisPort, int cmd)
{
     bool flagOK, readOK;
     writePort(thisPort, moduleTM->getAddrDecMod().toInt(), cmd, moduleTM->getTypeModule(), &flagOK);
     if(flagOK){
        readOK = getDataFromPort(thisPort, cmd, &flagOK, moduleTM->getTypeModule());
     }
     if(!flagOK){
         QMessageBox::information(NULL, "Сообщение", "При посылке команды произошла ошибка!", QMessageBox::Ok);
     }
     return (flagOK & readOK);
}
