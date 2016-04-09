#ifndef TICKETSSELLINGPOINT_VIEWMODEL_H
#define TICKETSSELLINGPOINT_VIEWMODEL_H

#include "tombolalib_global.h"
#include "TicketsBlock.h"
#include "BlockColorsSet_ViewModel.h"
#include "TicketsBlockSelection_ViewModel.h"

#include <QAbstractListModel>
#include <QColor>

#include <memory>
#include <vector>

class TombolaDocument;

class TOMBOLALIBSHARED_EXPORT TicketsSellingPoint_ViewModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TicketsSellingPoint_ViewModel(TombolaDocument& document, QObject *parent = 0);

    void Init(bool isPrizeDrawingRunning);

    enum TicketRoles
    {
        IsSoldRole = Qt::UserRole + 1
    };

    Q_INVOKABLE void componentOnCompleted();

    Q_PROPERTY(QString blockName READ getBlockName WRITE setBlockName NOTIFY blockNameChanged)
    Q_PROPERTY(int blockIndex READ getBlockIndex NOTIFY blockIndexChanged)
    Q_PROPERTY(QColor blockColor READ getBlockColor NOTIFY blockColorChanged)
    Q_PROPERTY(QColor textColor READ getTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QColor unsoldTicketColor READ getUnsoldTicketColor NOTIFY unsoldTicketColorChanged)

    Q_INVOKABLE void doColorSelection(int colorIndex);

    Q_INVOKABLE void beginRangeOperation(int startingTicketId);
    Q_INVOKABLE void rangeOperationInProgress(int ticketId);
    Q_INVOKABLE void endRangeOperation(int endTicketId);

    Q_PROPERTY(int totalTickets READ getTotalTickets NOTIFY totalTicketsChanged)
    Q_PROPERTY(int totalSoldTickets READ getTotalSoldTickets NOTIFY totalSoldTicketsChanged)
    Q_PROPERTY(int oneBlockSoldTickets READ getOneBlockSoldTickets NOTIFY oneBlockSoldTicketsChanged)
    Q_PROPERTY(int prizesCount READ prizesCount WRITE setPrizesCount NOTIFY prizesCountChanged)

    Q_PROPERTY(BlockColorsSet_ViewModel* blockColorsList READ getBlockColorsList NOTIFY blockColorsListChanged)
    Q_PROPERTY(TicketsBlockSelection_ViewModel* blockSelectionList READ getBlockSelectionList NOTIFY blockSelectionListChanged)

    Q_PROPERTY(bool canProceedToPrizeDrawing READ canProceedToPrizeDrawing NOTIFY canProceedToPrizeDrawingChanged)

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &, int role) override;

protected:
    QHash<int, QByteArray> roleNames() const;

    QString getBlockName() const;
    int getBlockIndex() const;
    QColor getBlockColor() const;
    QColor getTextColor() const;
    QColor getUnsoldTicketColor() const;

    void setBlockName(const QString& blockName);
    void setPrizesCount(int prizesCount);

    int getTotalTickets() const;
    int getTotalSoldTickets() const;
    int getOneBlockSoldTickets() const;
    int prizesCount() const;

    bool canProceedToPrizeDrawing() const;

    BlockColorsSet_ViewModel* getBlockColorsList() const;
    TicketsBlockSelection_ViewModel* getBlockSelectionList() const;

signals:
    void proceedToPrizeDrawingRequested();

    void colorsCountChanged();

    void blockNameChanged();
    void blockIndexChanged(int currentBlockIndex);
    void blockColorChanged();
    void textColorChanged();
    void unsoldTicketColorChanged();
    void prizesCountChanged();

    void totalTicketsChanged();
    void totalSoldTicketsChanged();
    void oneBlockSoldTicketsChanged();
    void canProceedToPrizeDrawingChanged();

    void blockColorsListChanged();
    void blockSelectionListChanged();

private slots:
    void onTicketsBlockSelected(int ticketsBlockIndex);
    void onTicketsBlocksInserted(const QModelIndex& parent, int first, int last);
    void onTicketsBlocksRemoved(const QModelIndex& parent, int first, int last);

private:
    TombolaDocument& m_TombolaDocument;
    std::shared_ptr<TicketsBlock> m_CurrentTicketsBlock;
    std::unique_ptr<BlockColorsSet_ViewModel> m_BlockColorsSet_ViewModel;
    std::unique_ptr<TicketsBlockSelection_ViewModel> m_TicketsBlockSelection_ViewModel;
    bool m_StartUpStraightIntoPrizeDrawing = false;

    void notifyAboutTicketsBlockChanged();
};

#endif // TICKETSSELLINGPOINT_VIEWMODEL_H
