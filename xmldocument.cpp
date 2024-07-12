
#include <QMessageBox>
#include <QObject>
#include <QDebug>

#include "xmldocument.h"

QMap<QString, int> XmlDocument::cfgMap;

XmlDocument::XmlDocument(QString file, QObject *parent):QObject(parent)
{
     fileXML = file;
}

XmlDocument::~XmlDocument()
{

}

void XmlDocument::parserCfgCom()
{
    OpenFileXML();
    getAttributes();
    closeFileXML();
}

void XmlDocument::saveCfgCom()
{
    OpenFileXML();
    setAttributes();
    closeFileXML();
}


void XmlDocument::OpenFileXML()
{
   xmlFile = new QFile(fileXML);
      if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::WriteOnly)) {
          QMessageBox::critical(NULL, "Ошибка загрузки файла XML",
                   "Невозможно открыть файл конфигурации com - портов:" + fileXML, QMessageBox::Ok);
         if(xmlFile) {delete xmlFile;}
         return;
      }
    //xmlReader = std::make_unique<QXmlStreamReader>(xmlFile);
}

void XmlDocument::closeFileXML()
{
    if(xmlReader){
       xmlReader->clear();
    }
    if(xmlFile){
       xmlFile->close();
    }
    delete xmlFile;
}

void XmlDocument::getAttributes()
{
    xmlReader = std::make_unique<QXmlStreamReader>(xmlFile);

    while(!xmlReader->atEnd() && !xmlReader->hasError()) {
          QXmlStreamReader::TokenType token = xmlReader->readNext();

         if(token == QXmlStreamReader::StartDocument) {
            continue;
         }
         if(token == QXmlStreamReader::StartElement) {
            if(xmlReader->name() == "Configuration") {
               parserConfig();
            }
        }
    }

    if(xmlReader->hasError()) {
            QMessageBox::critical(NULL, "Ошибка разбoра файла конфигурации портов.", xmlReader->errorString(), QMessageBox::Ok);
    }
    if(cfgMap.size() == 0) {
            QMessageBox::critical(NULL, "Ошибка разбoра файла конфигурации портов.", "Файл конфигурации пустой.", QMessageBox::Ok);
    }

}

void XmlDocument::parserConfig()
{
    QXmlStreamAttributes attributes = xmlReader->attributes();
    if (attributes.hasAttribute("Line1")){
        QString s = attributes.value("Line1").toString();
        cfgMap.insert("Line1", s.toInt());
    }
    if (attributes.hasAttribute("Line2")){
        QString s = attributes.value("Line2").toString();
        cfgMap.insert("Line2", s.toInt());
    }
    if (attributes.hasAttribute("BaudRate")){
        QString s = attributes.value("BaudRate").toString();
        cfgMap.insert("BaudRate", s.toInt());
    }
    if (attributes.hasAttribute("ByteSize")){
        QString s = attributes.value("ByteSize").toString();
        cfgMap.insert("ByteSize", s.toInt());
    }
    if (attributes.hasAttribute("Parity")){
        QString s = attributes.value("Parity").toString();
        cfgMap.insert("Parity", s.toInt());
    }
    if (attributes.hasAttribute("StopBit")){
        QString s = attributes.value("StopBit").toString();
        cfgMap.insert("StopBit", s.toInt());
    }
}

void XmlDocument::setAttributes()
{
    QXmlStreamWriter xmlWriter(xmlFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeComment("Файл конфигурации программы CMBB");
    xmlWriter.writeStartElement("Document");
    xmlWriter.writeComment("Конфигурация com-портов");
    xmlWriter.writeStartElement("Configuration");
    xmlWriter.writeAttribute("Line1", QString::number(cfgMap.value("Line1")));
    xmlWriter.writeAttribute("Line2", QString::number(cfgMap.value("Line2")));
    xmlWriter.writeAttribute("BaudRate", QString::number(cfgMap.value("BaudRate")));
    xmlWriter.writeAttribute("ByteSize", QString::number(cfgMap.value("ByteSize")));
    xmlWriter.writeAttribute("Parity", QString::number(cfgMap.value("Parity")));
    xmlWriter.writeAttribute("StopBit", QString::number(cfgMap.value("StopBit")));
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
}




