#include "logfile.h"

LogFile::LogFile(QString pathFile)
{
    logFile = pathFile;

    if(isDir()){
        if(!isFile()){
            QMessageBox::critical(nullptr, "Лог файл", "Произошла ошибка при открытии лог файла: "+logFile, QMessageBox::Ok);
        }
    }

}

LogFile::~LogFile()
{
    file->close();
    delete file;
}



void LogFile::appendString(QString msg)
{
    QDateTime time;
    QString str = msg + ";" + time.currentDateTime().toString();
    QByteArray be = str.toUtf8().append('\n');
    file->write(be);
}



QString LogFile::getHexFromBuf(char *buf, int size)
{
    QString str;
    QTextStream ts(&str);
    ts.setIntegerBase(16);
    ts.setFieldWidth(2);
    ts.setPadChar('0');
    for(int i=0; i<size; i++){
        ts<<static_cast<quint8>(buf[i])<<"  ";
    }
    return str;
}

void LogFile::addString(QString str)
{
    QByteArray be = str.toUtf8().append('\n');
    file->write(be);

}

QFile *LogFile::getFile() const
{
    return file;
}

bool LogFile::isFile()
{
    file = new QFile(logFile);
    file->open(QIODevice::Append | QIODevice::Text);
    return file->isOpen();
}

//Проверка на существование каталога. Разбирается строка пути файла//
//Если каталог не создан - создадим его.//
bool LogFile::isDir()
{
    QStringList list = logFile.split(QRegExp("[\\\\////]"));
    if(list.size() > 1){
        int len = list.at(list.size()-1).length();
        logDir  = logFile.left(logFile.length()-len);
        if(!dir.exists(logDir)){
            dir.mkdir(logDir);
        }
    }else{
        return true;//В параметре передан только файл//
    }
    return dir.exists(logDir);
}



