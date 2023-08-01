#ifndef DISCRITEM_H
#define DISCRITEM_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QColor>


class discretRDS;

class DiscrItem : public QWidget
{
    Q_OBJECT

public:
    explicit DiscrItem(quint32 id, QString hexAdrPlo, QWidget *parent = 0);
    ~DiscrItem();
    QVBoxLayout *getDiscrItem(){return vBoxItem;}

    bool getStateBtn() const;
    void setStateBtn(bool value);

    bool getStateInd() const;
    void setStateInd(bool value);

private:
    discretRDS *discrRds;
    QVBoxLayout *vBoxItem;
    QPixmap ind1, ind2;
    QLabel *indRead;
    QPushButton *pButton;
    QLabel      *lblAdr;
    quint32 itemId;
    quint32 num16;
    bool    stateBtn;
    int colorBtn[2]{0,1};
    QString adrPlo;
    quint32 comWord;
    bool stateInd;

signals:

public slots:
    void setColorItem(bool clr);
    void ctlStateButton();
    void setColorInd(bool clr);

};

#endif // DISCRITEM_H
