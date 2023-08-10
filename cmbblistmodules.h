#ifndef CMBBLISTMODULES_H
#define CMBBLISTMODULES_H

#include <QObject>
#include <QList>
#include <QAbstractTableModel>

#include "datamodules.h"

class CmbbListModules : public QAbstractTableModel
{
    Q_OBJECT

public:
    CmbbListModules(QObject *parent=nullptr);
    ~CmbbListModules();
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int getColumns() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    Qt::ItemFlags flags (const QModelIndex & index) const;
    void addCommand(DataModules& newData);
    void delCommand(const QModelIndex & index);
    DataModules &getDataModules(int index) const;
    void setDataModulesToModel();
    QList<DataModules> *getDataMod() const;
    void clearModel();


private:
    QList<DataModules> *dataMod;
    QVariant getData(int row, int col) const;
    int columns;

};

#endif // CMBBLISTMODULES_H
