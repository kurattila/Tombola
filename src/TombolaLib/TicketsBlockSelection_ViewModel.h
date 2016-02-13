#ifndef TICKETSBLOCKSELECTION_VIEWMODEL_H
#define TICKETSBLOCKSELECTION_VIEWMODEL_H

#include <QAbstractListModel>
#include <memory>

#include "tombolalib_global.h"
#include "TicketsBlock.h"
#include "TicketsBlocksSet.h"

class TicketsSellingPoint_ViewModel;
class TOMBOLALIBSHARED_EXPORT TicketsBlockSelection_ViewModel : public QAbstractListModel
{
    Q_OBJECT

    std::shared_ptr<TicketsBlocksSet> m_TicketsBlocksSet;
    std::shared_ptr<TicketsBlock> m_SelectedBlock;

//    void OnTicketsBlockAdded(std::shared_ptr<TicketsBlock> addedBlock);
//    void OnTicketsBlockRemoved(std::shared_ptr<TicketsBlock> removedBlock);
//    void OnTicketsBlockUpdated(std::shared_ptr<TicketsBlock> updatedBlock);

public:
    explicit TicketsBlockSelection_ViewModel(std::shared_ptr<TicketsBlocksSet> ticketsBlocksSet, TicketsSellingPoint_ViewModel* parentVM, QObject *parent = 0);

    enum TicketsBlockSelectionRoles
    {
        BackgroundColorRole = Qt::UserRole + 1,
        TextColorRole,
        NameRole,
        SoldTicketsRole,
        IsSelectedRole
    };

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE void doTicketsBlockSelection(int blockIndex);
    Q_INVOKABLE void doAddRow();
    Q_INVOKABLE void doDeleteRow(int ticketsBlockIndex);

protected:
    QHash<int, QByteArray> roleNames() const;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

signals:
    void rowSelected(int ticketsBlockIndex);

public slots:
//    void onCurrentBlockChanged(int currentBlockIndex);
    void onTicketsBlockDetailUpdated();
};

#endif // TICKETSBLOCKSELECTION_VIEWMODEL_H
