#ifndef TICKETDRAWEXECUTOR_H
#define TICKETDRAWEXECUTOR_H

#include "tombolalib_global.h"
#include "SingleTicketDraw_ViewModel.h"
#include "InGameTicketsRepository.h"

#include <memory>
#include <QObject>

class TombolaDocument;

class TOMBOLALIBSHARED_EXPORT TicketDrawExecutor : public QObject
{
    Q_OBJECT
public:
    explicit TicketDrawExecutor(  TombolaDocument& document
                                , SingleTicketDraw_ViewModel* ticketDrawLeft
                                , SingleTicketDraw_ViewModel* ticketDrawRight
                                , QObject *parent = 0);

    Q_INVOKABLE void onPrizeDrawingStartUp();
    Q_INVOKABLE void onTriggerByUser();

    Q_PROPERTY(SingleTicketDraw_ViewModel* ticketDrawLeft READ ticketDrawLeft NOTIFY ticketDrawLeftChanged)
    Q_PROPERTY(SingleTicketDraw_ViewModel* ticketDrawRight READ ticketDrawRight NOTIFY ticketDrawRightChanged)
    Q_PROPERTY(int remainingPrizesCount READ remainingPrizesCount NOTIFY remainingPrizesCountChanged)

private:
    TombolaDocument& m_Document;
    InGameTicketsRepository m_InGameTicketsRepository;
    std::unique_ptr<SingleTicketDraw_ViewModel> m_TicketDrawLeft;
    std::unique_ptr<SingleTicketDraw_ViewModel> m_TicketDrawRight;
    int m_RemainingPrizesCount = 0;
    int m_CurrentlySpinningCount = 0;

    SingleTicketDraw_ViewModel* ticketDrawLeft();
    SingleTicketDraw_ViewModel* ticketDrawRight();

    void setRemainingPrizesCount(int remaining);

protected:
    int remainingPrizesCount() const;

signals:
    void ticketDrawLeftChanged();
    void ticketDrawRightChanged();
    void remainingPrizesCountChanged();

public slots:
    void onTicketWinningPositionRequested();
};

#endif // TICKETDRAWEXECUTOR_H
