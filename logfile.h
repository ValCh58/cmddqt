#ifndef LOGFILE_H
#define LOGFILE_H

#include <QObject>
#include <QFile>
#include <QStringList>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QTextStream>

class LogFile
{
public:
    LogFile(QString pathFile);
    virtual ~LogFile();
    void appendString(QString msg);
    QString getHexFromBuf(char* buf, int size);
    void addString(QString str);
    QFile *getFile() const;


private:

    QString logDir;
    QString logFile;
    QFile *file;
    QDir dir;
    bool isFile();
    bool isDir();



};

#endif // LOGFILE_H
