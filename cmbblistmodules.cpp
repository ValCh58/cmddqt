#include <Qt>
#include <QColor>
#include <QFont>
#include <QDebug>

#include "cmbblistmodules.h"


CmbbListModules::CmbbListModules(QObject *parent=0):QAbstractTableModel(parent)
{
    columns = 11;
    dataMod = new QList<DataModules>();
}

CmbbListModules::~CmbbListModules()
{

}

int CmbbListModules::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return getColumns();
}

int CmbbListModules::getColumns() const
{
    return columns;
}

int CmbbListModules::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return dataMod->size();
}

QVariant CmbbListModules::data(const QModelIndex &index, int role) const
{
    switch(role){

    case Qt::DisplayRole: // Данные для отображения
    case Qt::EditRole: {   // Данные для редактирования
        if (index.isValid() && role == Qt::DisplayRole)
            return getData(index.row(), index.column());
    }
    case Qt::TextAlignmentRole: // Выравнивание
         return int(Qt::AlignHCenter | Qt::AlignVCenter);


    case Qt::BackgroundColorRole:{   // Цвет фона

        //if(index.row() % 2){
        //   return qVariantFromValue(QColor(255, 250, 250));
        //}else{
        //   return qVariantFromValue(QColor(238, 233, 233));
        //}
        DataModules dm = dataMod->at(index.row());
        int color = dm.getColorModuleRow();
        return qVariantFromValue(QColor(color));

    }
    case Qt::FontRole:{ // Шрифт
         QFont font = QFont("MS Sans Serif", -11, QFont::StyleItalic | QFont::Bold);
         return qVariantFromValue(font);

    }

    }

    return QVariant();
}

QVariant CmbbListModules::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal){
        switch (section) {

            case 0:
                return QVariant("№\n Линии");
            case 1:
                return QVariant("Внутренний\n номер");
            case 2:
                return QVariant("Адрес");
            case 3:
                return QVariant("Адрес\n Hex");
            case 4:
                return QVariant("ПЛО");
            case 5:
                return QVariant("Тип");
            case 6:
                return QVariant("Инвер-\n сия");
            case 7:
                return QVariant("Усред-\n нение");
            case 8:
                return QVariant("Версия\n ПО");
            case 9:
                return QVariant("ID");
            case 10:
                return QVariant("Резервный\n модуль");
            default:
                return QVariant();
        }
    }

    return QVariant();
}

Qt::ItemFlags CmbbListModules::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void CmbbListModules::addCommand(DataModules &newData)
{
    beginInsertRows(QModelIndex(), dataMod->size(), dataMod->size());
    dataMod->append(newData);
    endInsertRows();
}

void CmbbListModules::delCommand(const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    dataMod->removeAt(index.row());
    endRemoveRows();
}

DataModules &CmbbListModules::getDataModules(int index) const
{
    return(*dataMod)[index];
}

void CmbbListModules::setDataModulesToModel()
{
  qDebug()<<"Debug=>setDataModulesToModel";
}
QList<DataModules> *CmbbListModules::getDataMod() const
{
    return dataMod;
}

//Очистка модели от данных//
void CmbbListModules::clearModel()
{
    if(dataMod->size()>0){
       beginResetModel();
       dataMod->clear();
       endResetModel();
    }
}

QVariant CmbbListModules::getData(int row, int col) const
{
    switch(col){
    case 0:
        return QVariant(dataMod->at(row).getNumLine());
    case 1:
        return QVariant(dataMod->at(row).getNumInternal());
    case 2:
        return QVariant(dataMod->at(row).getAddrDecMod());
    case 3:
        return QVariant(dataMod->at(row).getAddrHexMod());
    case 4:
        return QVariant(dataMod->at(row).getAddrPLO());
    case 5:
        return QVariant(dataMod->at(row).getTypeMod());
    case 6:
        return QVariant(dataMod->at(row).getStatusInver());
    case 7:
        return QVariant(dataMod->at(row).getSetAver());
    case 8:
        return QVariant(dataMod->at(row).getVersionMod());
    case 9:
        return QVariant(dataMod->at(row).getIdMod());
    case 10:
        return QVariant(dataMod->at(row).getReservMod());
    default:
        return QVariant();
    }
}



