#ifndef DISCRETRDS_H
#define DISCRETRDS_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>

#include "datamodules.h"
#include "discritem.h"



class discretRDS : public QWidget
{
    Q_OBJECT

public:
    explicit discretRDS(int type, DataModules mod, QList<DataModules> list,QWidget *parent = 0);
    ~discretRDS();

    quint32 getComWord() const;
    void setComWord(const quint32 &value);
    DataModules getModule() const;

    QLineEdit *getTxtWrite() const;
    void setTxtWrite(QLineEdit *value);

    QLineEdit *getTxtRead() const;
    void setTxtRead(QLineEdit *value);

    quint32 getReadWord() const;
    void setReadWord(const quint32 &value);

    DataModules getModNNC() const;

    int getLineNNC() const;


private:
    QPushButton *btnClear;//Очистить выбор
    QPushButton *btnCheckAll;//Отметить всё

    QLabel *labelAddr;
    QLineEdit *txtAddr;
    QLabel *labelAddrHex;
    QLineEdit *txtAddrHex;
    QLabel *labelPlo;
    QLineEdit *txtPlo;
    QLabel *labelWrite;
    QLineEdit *txtWrite;
    QLabel *labelRead;
    QLineEdit *txtRead;
    QLabel *labelNNC;
    QComboBox *cmbMnc;
    QLabel *labelSpLine;
    QSpinBox *spNumLine;

    void createItems();
    QMap<quint32, DiscrItem*> mapItems;
    QHBoxLayout *hBtn;

    void setStateBtn(bool b);
    void stateBtn(bool b);
    quint32 comWord;//Слово данных посылки в РДС//
    quint32 readWord;//Слово ответа от РДС//
    QString ploHexToHex(QString strHex, quint8 idx);
    DataModules module;//РДС//
    DataModules modNNC;//ННС//
    QList<DataModules> *nncList;
    int lineNNC;
    QHBoxLayout *greateHBox1(int type);
    QHBoxLayout *greateHBox2();

signals:
    void sigAllOnOff(bool);

public slots:
    void allOn();
    void allOff();
    void setIndItemRDS(quint32 wrd);
    void setDataModNNC(int idx);
    void setNumLineNNC(int line);

};

#endif // DISCRETRDS_H
