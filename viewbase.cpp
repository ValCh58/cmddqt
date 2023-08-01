#include "viewbase.h"

ViewBase::ViewBase(QWidget *parent):QTableView(parent)
{
    dynResizeTable();
    setInitStyle();
}

void ViewBase::headerSize()
{

}

void ViewBase::dynResizeTable()
{
    QSizePolicy policy = sizePolicy();
    policy.setVerticalStretch(1);
    policy.setHorizontalStretch(1);
    setSizePolicy(policy);
}

void ViewBase::setInitStyle()
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ContiguousSelection);
    setAlternatingRowColors(true);
    setStyleSheet("QTableView::item {selection-color: yellow; selection-background-color: #2F4F4F;}");
    setCornerButtonEnabled(false);//Верхний левый угол - выбор всех ячеек ОТКЛЮЧЕН!//
}

