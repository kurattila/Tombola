#include "TicketsBlockSelection_ViewModel.h"
#include "TicketsSellingPoint_ViewModel.h"
#include "BlockColorsSet.h"

TicketsBlockSelection_ViewModel::TicketsBlockSelection_ViewModel(std::shared_ptr<TicketsBlocksSet> ticketsBlocksSet, TicketsSellingPoint_ViewModel *parentVM, QObject *parent)
    : QAbstractListModel(parent)
    , m_TicketsBlocksSet(ticketsBlocksSet)
{
//    connect(parentVM, SIGNAL(blockIndexChanged(int)), this, SLOT(onCurrentBlockChanged(int)));
    connect(parentVM, SIGNAL(oneBlockSoldTicketsChanged()), this, SLOT(onTicketsBlockDetailUpdated()));
    connect(parentVM, SIGNAL(blockNameChanged()), this, SLOT(onTicketsBlockDetailUpdated()));
    connect(parentVM, SIGNAL(blockColorChanged()), this, SLOT(onTicketsBlockDetailUpdated()));
}

int TicketsBlockSelection_ViewModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_TicketsBlocksSet->GetBlocksCount();
}

QVariant TicketsBlockSelection_ViewModel::data(const QModelIndex &index, int role) const
{
    auto currentBlock = m_TicketsBlocksSet->GetBlock(index.row());
    if (role == BackgroundColorRole)
        return BlockColorsSet::GetBlockColor(currentBlock->ColorsSetIndex);
    else if (role == TextColorRole)
        return BlockColorsSet::GetTextColor(currentBlock->ColorsSetIndex);
    else if (role == NameRole)
        return currentBlock->Name;
    else if (role == SoldTicketsRole)
        return static_cast<int>(currentBlock->GetSoldTicketIds().size());
    else if (role == IsSelectedRole)
        return currentBlock == m_SelectedBlock;
    else
        return false;
}

void TicketsBlockSelection_ViewModel::doTicketsBlockSelection(int blockIndex)
{
    m_SelectedBlock = m_TicketsBlocksSet->GetBlock(blockIndex);
    emit dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 0));
//    emit dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 0), QVector<int>{IsSelectedRole});
//    emit dataChanged(createIndex(selectedIndex, 0), createIndex(selectedIndex, 0));
    emit rowSelected(blockIndex);
}

void TicketsBlockSelection_ViewModel::doAddRow()
{
    insertRow(rowCount());

    int newSelectedBlockIndex = rowCount() - 1;
    doTicketsBlockSelection(newSelectedBlockIndex);
}


void TicketsBlockSelection_ViewModel::doDeleteRow(int ticketsBlockIndex)
{
    removeRow(ticketsBlockIndex);

    int newSelectedBlockIndex = m_TicketsBlocksSet->FindBlockIndex(m_SelectedBlock);
    doTicketsBlockSelection(newSelectedBlockIndex);
}

//void TicketsBlockSelection_ViewModel::OnTicketsBlockAdded(std::shared_ptr<TicketsBlock> addedBlock)
//{
////    beginInsertRows();
//}

//void TicketsBlockSelection_ViewModel::OnTicketsBlockRemoved(std::shared_ptr<TicketsBlock> removedBlock)
//{

//}

//void TicketsBlockSelection_ViewModel::OnTicketsBlockUpdated(std::shared_ptr<TicketsBlock> updatedBlock)
//{
//    emit dataChanged(createIndex(0,0), createIndex(rowCount()-1, 0));
//}

QHash<int, QByteArray> TicketsBlockSelection_ViewModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[BackgroundColorRole] = "backgroundColor";
    roles[TextColorRole] = "textColor";
    roles[NameRole] = "name";
    roles[SoldTicketsRole] = "soldTickets";
    roles[IsSelectedRole] = "isSelected";
    return roles;
}

bool TicketsBlockSelection_ViewModel::insertRows(int row, int count, const QModelIndex& parent)
{
    int colorsSetIndexForNewBlocks = m_SelectedBlock->ColorsSetIndex;

    beginInsertRows(parent, row, row + count - 1);
    for (int i = row; i < row + count; ++ i)
    {
        m_SelectedBlock = m_TicketsBlocksSet->AddBlock();
        m_SelectedBlock->ColorsSetIndex = colorsSetIndexForNewBlocks;
    }
    endInsertRows();

    return true;
}

bool TicketsBlockSelection_ViewModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    while(count--)
    {
        auto deletedBlock = m_TicketsBlocksSet->GetBlock(row);
        m_SelectedBlock = m_TicketsBlocksSet->DeleteBlock(deletedBlock);
    }
    endRemoveRows();

    return true;
}

//void TicketsBlockSelection_ViewModel::onCurrentBlockChanged(int currentBlockIndex)
//{
//    m_SelectedBlock = m_TicketsBlocksSet->GetBlock(currentBlockIndex);
//    emit dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 0), QVector<int>{IsSelectedRole});
//}

void TicketsBlockSelection_ViewModel::onTicketsBlockDetailUpdated()
{
    int selectedIndex = m_TicketsBlocksSet->FindBlockIndex(m_SelectedBlock);
    emit dataChanged(createIndex(selectedIndex, 0), createIndex(selectedIndex, 0));
}
