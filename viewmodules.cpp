#include "viewmodules.h"

/**
 * @brief ViewModules::ViewModules
 * @param parent
 */
ViewModules::ViewModules(QWidget *parent):ViewBase(parent)
{

}

/**
 * @brief ViewModules::~ViewModules
 */
ViewModules::~ViewModules() {}

/**
 * @brief ViewModules::headerSize
 */
void ViewModules::headerSize()
{
    QHeaderView *vheader = verticalHeader();
    vheader->setDefaultSectionSize(20);

    QHeaderView *hheader = horizontalHeader();
    hheader->setStyleSheet("QHeaderView::section { background-color: rgb(200,200,200);font: 12px;font-weight: normal;}");
    hheader->resizeSection(0,70);
    hheader->resizeSection(1,88);
    hheader->resizeSection(2,55);
    hheader->resizeSection(3,55);
    hheader->resizeSection(4,60);
    hheader->resizeSection(5,85);
    hheader->resizeSection(6,55);
    hheader->resizeSection(7,65);
    hheader->resizeSection(8,70);
    hheader->resizeSection(9,90);
    hheader->resizeSection(10,80);
}


