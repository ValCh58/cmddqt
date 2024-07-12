#ifndef VIEWBASE_H
#define VIEWBASE_H

#include <QObject>
#include <QTableView>

class ViewBase : public QTableView
{
public:
    ViewBase(QWidget *parent);
    virtual ~ViewBase();

private:
    void dynResizeTable();
    void setInitStyle();
};

#endif // VIEWBASE_H
