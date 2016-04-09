#ifndef TICKETDRAWEXECUTOR_H
#define TICKETDRAWEXECUTOR_H

#include "tombolalib_global.h"
#include "SingleTicketDraw_ViewModel.h"
#include "InGameTicketsRepository.h"
#include "IMemento.h"

#include <memory>
#include <QObject>

class TombolaDocument;

class TOMBOLALIBSHARED_EXPORT TicketDrawExecutor : public QObject
                                                 , public IMementoOriginator
{
    Q_OBJECT
public:
    explicit TicketDrawExecutor(  TombolaDocument& document
                                , SingleTicketDraw_ViewModel* ticketDrawLeft
                                , SingleTicketDraw_ViewModel* ticketDrawRight
                                , QObject *parent = 0);

    Q_INVOKABLE void onPrizeDrawingStartUp();
    Q_INVOKABLE void onTriggerByUser();
    Q_INVOKABLE void onPrizeDrawingAborted();

    Q_PROPERTY(SingleTicketDraw_ViewModel* ticketDrawLeft READ ticketDrawLeft NOTIFY ticketDrawLeftChanged)
    Q_PROPERTY(SingleTicketDraw_ViewModel* ticketDrawRight READ ticketDrawRight NOTIFY ticketDrawRightChanged)
    Q_PROPERTY(int remainingPrizesCount READ remainingPrizesCount WRITE setRemainingPrizesCount NOTIFY remainingPrizesCountChanged)
    Q_PROPERTY(int minAllowedRemainingPrizesCount READ minAllowedRemainingPrizesCount NOTIFY minAllowedRemainingPrizesCountChanged)

    bool IsPrizeDrawingRunning() const;

    // IMementoOriginator interface
    virtual IMemento* SaveToMemento() override;
    virtual void RestoreFromMemento(const IMemento* memento, void* context = nullptr) override;

private:
    TombolaDocument& m_Document;
    InGameTicketsRepository m_InGameTicketsRepository;
    std::unique_ptr<SingleTicketDraw_ViewModel> m_TicketDrawLeft;
    std::unique_ptr<SingleTicketDraw_ViewModel> m_TicketDrawRight;
    int m_RemainingPrizesCount = 0;
    int m_CurrentlySpinningCount = 0;
    bool m_PrizeDrawingRunning = false;

    SingleTicketDraw_ViewModel* ticketDrawLeft();
    SingleTicketDraw_ViewModel* ticketDrawRight();

    void setRemainingPrizesCount(int remaining);
    void setCurrentlySpinningCount(int ticketsSpinningNow);

protected:
    int remainingPrizesCount() const;
    int minAllowedRemainingPrizesCount() const;
    int maxAllowedRemainingPrizesCount() const;

signals:
    void ticketDrawLeftChanged();
    void ticketDrawRightChanged();
    void remainingPrizesCountChanged();
    void minAllowedRemainingPrizesCountChanged();

public slots:
    void onTicketWinningPositionRequested(const std::shared_ptr<Ticket>& ticket);
};

class TOMBOLALIBSHARED_EXPORT TicketDrawExecutorMemento : public IMemento
{
    friend class TicketDrawExecutor;

    bool PrizeDrawingRunning = false;
    int CurrentlySpinningCount = 0;
    int RemainingPrizesCount = 0;
    std::unique_ptr<IMemento> InGameTicketsRepoMemento;

public:
    static QString StartElementXmlName;

    TicketDrawExecutorMemento();
    virtual ~TicketDrawExecutorMemento();

    // IMemento interface
    virtual void Read(QXmlStreamReader& xmlReader);
    virtual void Write(QXmlStreamWriter& xmlWriter);
};

#endif // TICKETDRAWEXECUTOR_H
