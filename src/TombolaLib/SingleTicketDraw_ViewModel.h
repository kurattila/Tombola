#ifndef SINGLETICKETDRAW_VIEWMODEL_H
#define SINGLETICKETDRAW_VIEWMODEL_H

#include <QAbstractListModel>
#include <QColor>

#include <memory>

#include "tombolalib_global.h"
#include "Ticket.h"
#include "RandomTicketDraw.h"
#include "SingleTicketDraw_ViewModel.h"

class InGameTicketsRepository;

class TOMBOLALIBSHARED_EXPORT SingleTicketDraw_ViewModel : public QAbstractListModel
{
    Q_OBJECT
    RandomTicketDraw m_RandomTicketDraw;
    int m_CurrentFlyThroughCounter = 0;
    int m_FlyThroughAnimationDuration = 0;
    static const int FlyThroughAnimationDurationStartValue = 300;
    static const int DefaultStepsCountOfDraw = 12;
    std::list<std::shared_ptr<Ticket>> m_WinningTicketsHistory;

    enum class TicketDrawState {
        NotStarted,
        AwaitingWinningTicket,
        WinningTicketShown
    };

    TicketDrawState m_TicketDrawState = TicketDrawState::NotStarted;

public:
    explicit SingleTicketDraw_ViewModel(QObject *parent = 0);

    virtual void Init(InGameTicketsRepository* inGameTicketsRepository);

    enum class ResultOfUserTrigger
    {
        Accepted,
        Rejected
    };

    enum WinningTicketsHistoryRoles
    {
        BackgroundColorRole = Qt::UserRole + 1,
        TextColorRole,
        NameRole,
        TicketIdRole
    };

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

//    Q_INVOKABLE void onPrizeDrawingStartUp();
    virtual ResultOfUserTrigger onTriggerByUser();
    void onStartSingleTicketDraw();

    Q_INVOKABLE void onFlyUpTicketStateAchieved();
    Q_INVOKABLE void onWinningTicketStateAchieved();
    Q_INVOKABLE void onVanishedTicketStateAchieved();

    Q_INVOKABLE void removeFromWinningTicketsHistory(int index);

    Q_PROPERTY(QColor flyingTicketBgColor READ flyingTicketBgColor NOTIFY flyingTicketBgColorChanged)
    Q_PROPERTY(QColor flyingTicketTextColor READ flyingTicketTextColor NOTIFY flyingTicketTextColorChanged)
    Q_PROPERTY(QString flyingTicketBlockName READ flyingTicketBlockName NOTIFY flyingTicketBlockNameChanged)
    Q_PROPERTY(int flyingTicketId READ flyingTicketId NOTIFY flyingTicketIdChanged)
    Q_PROPERTY(int animationDuration READ animationDuration NOTIFY animationDurationChanged)

    const QColor& flyingTicketBgColor() const;
    const QColor& flyingTicketTextColor() const;
    const QString& flyingTicketBlockName() const;
    int flyingTicketId() const;
    int animationDuration() const;

protected:
    QHash<int, QByteArray> roleNames() const;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    void setCurrentFlyThroughTicket(std::shared_ptr<Ticket> ticket);

    InGameTicketsRepository* m_InGameTicketsRepository = nullptr;
    std::shared_ptr<Ticket> m_CurrentFlyThroughTicket;

signals:
    void ticketStartupPositionRequested();
    void ticketFlyThroughRequested();
    void ticketWinningPositionRequested();
    void ticketVanishRequested();

    void flyingTicketBgColorChanged();
    void flyingTicketTextColorChanged();
    void flyingTicketBlockNameChanged();
    void flyingTicketIdChanged();
    void animationDurationChanged();

public slots:
};

#endif // SINGLETICKETDRAW_VIEWMODEL_H
