#include "BlockColorsSet_ViewModel.h"
#include "BlockColorsSet.h"

BlockColorsSet_ViewModel::BlockColorsSet_ViewModel(QObject *parent) : QAbstractListModel(parent)
{

}

int BlockColorsSet_ViewModel::rowCount(const QModelIndex& /*parent*/) const
{
    return getColorsCount();
}

QVariant BlockColorsSet_ViewModel::data(const QModelIndex &index, int /*role*/) const
{
    const QColor& refColor = BlockColorsSet::GetBlockColor(index.row());
    return refColor;
}

QHash<int, QByteArray> BlockColorsSet_ViewModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[BackgroundColorRole] = "backgroundColor";
    return roles;
}

int BlockColorsSet_ViewModel::getColorsCount() const
{
    return BlockColorsSet::GetColorsCount();
}
