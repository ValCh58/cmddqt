#include "viewmodules.h"

ViewModules::ViewModules(QWidget *parent):ViewBase(parent)
{

}

ViewModules::~ViewModules()
{

}

void ViewModules::headerSize()
{
    QHeaderView *vheader = verticalHeader();
    vheader->setDefaultSectionSize(20);

    QHeaderView *hheader = horizontalHeader();
    hheader->setStyleSheet("QHeaderView::section { background-color: rgb(200,200,200);font: 12px;font-weight: bold;}");
    hheader->resizeSection(0,60);
    hheader->resizeSection(1,80);
    hheader->resizeSection(2,50);
    hheader->resizeSection(3,50);
    hheader->resizeSection(4,50);
    hheader->resizeSection(5,50);
    hheader->resizeSection(6,50);
    hheader->resizeSection(7,65);
    hheader->resizeSection(8,70);
    hheader->resizeSection(9,70);
    hheader->resizeSection(10,80);
}

