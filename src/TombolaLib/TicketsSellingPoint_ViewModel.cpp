#include "TicketsSellingPoint_ViewModel.h"
#include "TombolaDocument.h"
#include "BlockColorsSet.h"
#include "TicketsBlockFactory.h"

TicketsSellingPoint_ViewModel::TicketsSellingPoint_ViewModel(TombolaDocument& document, QObject *parent)
    : QAbstractListModel(parent)
    , m_TombolaDocument(document)
{
    m_BlockColorsSet_ViewModel.reset(new BlockColorsSet_ViewModel(parent));
    m_TicketsBlockSelection_Sorted_ProxyModel.reset(new TicketsBlockSelection_SortFilterProxyModel(m_TombolaDocument.AllTicketsBlocksSet, this, parent));
}

void TicketsSellingPoint_ViewModel::Init(bool isPrizeDrawingRunning)
{
    m_StartUpStraightIntoPrizeDrawing = isPrizeDrawingRunning;
    m_CurrentTicketsBlock = m_TombolaDocument.AllTicketsBlocksSet->GetFirstBlock();

    m_TicketsBlockSelection_Sorted_ProxyModel->doTicketsBlockSelection(getBlockIndexSorted());

    connect(m_TicketsBlockSelection_Sorted_ProxyModel->sourceModel(), SIGNAL(rowSelected(int)), this, SLOT(onNonSortedTicketsBlockSelected(int)));
    connect(m_TicketsBlockSelection_Sorted_ProxyModel->sourceModel(), SIGNAL(rowsInserted(const QModelIndex&,int,int)), this, SLOT(onNonSortedTicketsBlocksInserted(const QModelIndex&,int,int)));
    connect(m_TicketsBlockSelection_Sorted_ProxyModel->sourceModel(), SIGNAL(rowsRemoved(const QModelIndex&,int,int)), this, SLOT(onNonSortedTicketsBlocksRemoved(const QModelIndex&,int,int)));
}

void TicketsSellingPoint_ViewModel::componentOnCompleted()
{
    if (m_StartUpStraightIntoPrizeDrawing)
        emit proceedToPrizeDrawingRequested();
}

void TicketsSellingPoint_ViewModel::doColorSelection(int colorIndex)
{
    m_CurrentTicketsBlock->ColorsSetIndex = colorIndex;
    notifyAboutTicketsBlockChanged();
}

void TicketsSellingPoint_ViewModel::beginRangeOperation(int startingTicketId)
{
    m_CurrentTicketsBlock->BeginRangeOperation(startingTicketId);
    rangeOperationInProgress(startingTicketId);
}

void TicketsSellingPoint_ViewModel::rangeOperationInProgress(int ticketId)
{
    m_CurrentTicketsBlock->RangeOperationInProgress(ticketId);

    emit dataChanged(createIndex(0, 0)
                   , createIndex(rowCount() - 1, 0));
    emit oneBlockSoldTicketsChanged();
    emit totalSoldTicketsChanged();
    emit canProceedToPrizeDrawingChanged();
}

void TicketsSellingPoint_ViewModel::endRangeOperation(int endTicketId)
{
    rangeOperationInProgress(endTicketId);
    m_CurrentTicketsBlock->EndRangeOperation(endTicketId);
}

int TicketsSellingPoint_ViewModel::rowCount(const QModelIndex& /*parent*/) const
{
    return TicketsBlockFactory::DefaultTicketsPerBlock;
}

QVariant TicketsSellingPoint_ViewModel::data(const QModelIndex& index, int role) const
{
    int ticketId = index.row() + 1;

    if (role == IsSoldRole)
    {
        auto itFound = m_CurrentTicketsBlock->GetSoldTicketIds().find(ticketId);
        if (itFound != m_CurrentTicketsBlock->GetSoldTicketIds().end())
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}

Qt::ItemFlags TicketsSellingPoint_ViewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool TicketsSellingPoint_ViewModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    int ticketId = index.row() + 1;
    if (role == IsSoldRole)
    {
        m_CurrentTicketsBlock->SetTicketSold(ticketId, value.toBool());
        emit dataChanged(index, index);
        emit totalSoldTicketsChanged();
        emit oneBlockSoldTicketsChanged();
        emit canProceedToPrizeDrawingChanged();

        return true;
    }
    else
    {
        return false;
    }
}

QHash<int, QByteArray> TicketsSellingPoint_ViewModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IsSoldRole] = "isSold";
    return roles;
}

QString TicketsSellingPoint_ViewModel::getBlockName() const
{
    return m_CurrentTicketsBlock->Name;
}

int TicketsSellingPoint_ViewModel::getBlockIndexSorted() const
{
    int blockRawIndex = m_TombolaDocument.AllTicketsBlocksSet->FindBlockIndex(m_CurrentTicketsBlock);
    int blockIndexSorted = m_TicketsBlockSelection_Sorted_ProxyModel->MapBlockIndexFromSource(blockRawIndex);
    return blockIndexSorted;
}

QColor TicketsSellingPoint_ViewModel::getBlockColor() const
{
    return BlockColorsSet::GetBlockColor( m_CurrentTicketsBlock->ColorsSetIndex );
}

QColor TicketsSellingPoint_ViewModel::getTextColor() const
{
    return BlockColorsSet::GetTextColor( m_CurrentTicketsBlock->ColorsSetIndex );
}

QColor TicketsSellingPoint_ViewModel::getUnsoldTicketColor() const
{
    return BlockColorsSet::GetUnsoldTicketColor(m_CurrentTicketsBlock->ColorsSetIndex );
}

void TicketsSellingPoint_ViewModel::setBlockName(const QString& blockName)
{
    if (m_CurrentTicketsBlock->Name != blockName) // avoid message of 'QML TextInput: Binding loop detected for property "text"'
    {
        m_CurrentTicketsBlock->Name = blockName;
        emit blockNameChanged(); // emit needed for connect(...)-based slots to be triggered (as used e.g. in TicketsBlockSelection_ViewModel)
        emit blockIndexSortedChanged( getBlockIndexSorted() ); // needed by auto-scrolling the ListView to the block which is currently being renamed
    }
}

void TicketsSellingPoint_ViewModel::setPrizesCount(int prizesCount)
{
    m_TombolaDocument.PrizesCount = prizesCount;
    emit canProceedToPrizeDrawingChanged();
}

int TicketsSellingPoint_ViewModel::getTotalTickets() const
{
    return m_TombolaDocument.AllTicketsBlocksSet->GetBlocksCount() * TicketsBlockFactory::DefaultTicketsPerBlock;
}

int TicketsSellingPoint_ViewModel::getTotalSoldTickets() const
{
    int totalSold = 0;
    auto block = m_TombolaDocument.AllTicketsBlocksSet->GetFirstBlock();
    while (block.get())
    {
        totalSold += static_cast<int>(block->GetSoldTicketIds().size());
        block = m_TombolaDocument.AllTicketsBlocksSet->GetNextBlock(block);
    }
    return totalSold;
}

int TicketsSellingPoint_ViewModel::getOneBlockSoldTickets() const
{
    return static_cast<int>(m_CurrentTicketsBlock->GetSoldTicketIds().size());
}

int TicketsSellingPoint_ViewModel::prizesCount() const
{
    return m_TombolaDocument.PrizesCount;
}

bool TicketsSellingPoint_ViewModel::canProceedToPrizeDrawing() const
{
    return getTotalSoldTickets() >= m_TombolaDocument.PrizesCount;
}

BlockColorsSet_ViewModel* TicketsSellingPoint_ViewModel::getBlockColorsList() const
{
    return m_BlockColorsSet_ViewModel.get();
}

TicketsBlockSelection_SortFilterProxyModel* TicketsSellingPoint_ViewModel::getBlockSelectionList() const
{
    return m_TicketsBlockSelection_Sorted_ProxyModel.get();
}

void TicketsSellingPoint_ViewModel::onNonSortedTicketsBlockSelected(int ticketsBlockIndexNonSorted)
{
    m_CurrentTicketsBlock = m_TombolaDocument.AllTicketsBlocksSet->GetBlock(ticketsBlockIndexNonSorted);

    notifyAboutTicketsBlockChanged();
}

void TicketsSellingPoint_ViewModel::onNonSortedTicketsBlocksInserted(const QModelIndex& /*parent*/, int /*firstNonSorted*/, int lastNonSorted)
{
    m_CurrentTicketsBlock = m_TombolaDocument.AllTicketsBlocksSet->GetBlock(lastNonSorted);

    notifyAboutTicketsBlockChanged();
    emit totalTicketsChanged();
    emit totalSoldTicketsChanged();
    emit canProceedToPrizeDrawingChanged();
}

void TicketsSellingPoint_ViewModel::onNonSortedTicketsBlocksRemoved(const QModelIndex& /*parent*/, int /*firstNonSorted*/, int /*lastNonSorted*/)
{
    emit totalTicketsChanged();
    emit totalSoldTicketsChanged();
    emit canProceedToPrizeDrawingChanged();
}

void TicketsSellingPoint_ViewModel::notifyAboutTicketsBlockChanged()
{
    emit blockIndexSortedChanged( getBlockIndexSorted() );
    emit blockNameChanged();
    emit blockColorChanged();
    emit textColorChanged();

    emit oneBlockSoldTicketsChanged();

    emit dataChanged(createIndex(0, 0)
                   , createIndex(rowCount() - 1, 0));
}
