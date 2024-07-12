#ifndef VIEWMODULES_H
#define VIEWMODULES_H

#include <QTableView>
#include <QHeaderView>
#include "viewbase.h"

class ViewModules : public ViewBase
{
public:
    ViewModules(QWidget *parent = 0);
    virtual ~ViewModules();
    void headerSize();

private:

};

#endif // VIEWMODULES_H
