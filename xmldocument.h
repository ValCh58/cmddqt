#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H

#include <memory>
#include <QObject>
#include <QFile>
#include <QXmlStreamReader>

class XmlDocument : public QObject
{
    Q_OBJECT

public:
    explicit XmlDocument(QString file, QObject *parent=0);
    virtual ~XmlDocument();
    void parserCfgCom();
    void saveCfgCom();
    static QMap<QString, int> cfgMap;

private:
    QString fileXML;
    QFile* xmlFile = nullptr;
    std::unique_ptr<QXmlStreamReader> xmlReader = nullptr;
    void OpenFileXML();
    void closeFileXML();
    void getAttributes();
    void parserConfig();
    void setAttributes();



};

#endif // XMLDOCUMENT_H
