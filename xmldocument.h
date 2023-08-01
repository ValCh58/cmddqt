#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>

class XmlDocument : public QObject
{
    Q_OBJECT

public:
    explicit XmlDocument(QString file, QObject *parent=0);
    ~XmlDocument();
    void parserCfgCom();
    void saveCfgCom();
    static QMap<QString, int> cfgMap;

private:
    QString fileXML;
    QFile *xmlFile;
    QXmlStreamReader *xmlReader;
    void OpenFileXML();
    void closeFileXML();
    void getAttributes();
    void parserConfig();
    void setAttributes();



};

#endif // XMLDOCUMENT_H
