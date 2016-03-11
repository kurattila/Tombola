#include "SingleTicketDraw_ViewModel.h"
#include "BlockColorsSet.h"
#include "InGameTicketsRepository.h"

SingleTicketDraw_ViewModel::SingleTicketDraw_ViewModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void SingleTicketDraw_ViewModel::Init(InGameTicketsRepository *inGameTicketsRepository)
{
    removeRows(0, rowCount()); // forget any previous prize drawings
    m_InGameTicketsRepository = inGameTicketsRepository;

    emit ticketStartupPositionRequested();
    m_TicketDrawState = TicketDrawState::NotStarted;
}

int SingleTicketDraw_ViewModel::rowCount(const QModelIndex& /*parent*/) const
{
    return static_cast<int>(m_WinningTicketsHistory.size());
}

QVariant SingleTicketDraw_ViewModel::data(const QModelIndex& index, int role) const
{
    int winningTicketAge = index.row();
    auto itRefTicket = std::next(m_WinningTicketsHistory.begin(), winningTicketAge);
    std::shared_ptr<Ticket> refTicket = *itRefTicket;
    if (!refTicket)
        return false;

    int colorIndex = refTicket->ColorsSetIndex();
    switch(role)
    {
    case BackgroundColorRole:
        return BlockColorsSet::GetBlockColor(colorIndex);
        break;
    case TextColorRole:
        return BlockColorsSet::GetTextColor(colorIndex);
        break;
    case NameRole:
        return refTicket->BlockName();
        break;
    case TicketIdRole:
        return refTicket->TicketNumber();
        break;
    }

    return false;
}

//void SingleTicketDraw_ViewModel::onPrizeDrawingStartUp()
//{
//    Init();
//    onTriggerByUser();
//}

SingleTicketDraw_ViewModel::ResultOfUserTrigger SingleTicketDraw_ViewModel::onTriggerByUser()
{
    ResultOfUserTrigger result = ResultOfUserTrigger::Rejected;
    switch(m_TicketDrawState)
    {
    case TicketDrawState::NotStarted:
        onStartSingleTicketDraw();
        result = ResultOfUserTrigger::Accepted;
        break;
    case TicketDrawState::AwaitingWinningTicket:
        result = ResultOfUserTrigger::Rejected;
        break; // reject user input while ticket drawing still in progress
    case TicketDrawState::WinningTicketShown:
        emit ticketVanishRequested();
        result = ResultOfUserTrigger::Accepted;
        m_TicketDrawState = TicketDrawState::AwaitingWinningTicket; // important! without this, pressing and holding 'space' for 30 seconds
                                                                    // will make prize drawing mess up the whole state and stop prize drawing altogether!!!
        break;
    }

    return result;
}

void SingleTicketDraw_ViewModel::onStartSingleTicketDraw()
{
    m_TicketDrawState = TicketDrawState::AwaitingWinningTicket;

    m_CurrentFlyThroughCounter = 0;
    emit animationDurationChanged();

    auto untouchedTickets = m_InGameTicketsRepository->GetTicketsStillInGame();
    m_RandomTicketDraw.Init(SingleTicketDraw_ViewModel::DefaultStepsCountOfDraw, untouchedTickets);
    auto winningTicket = m_RandomTicketDraw.GetWinningTicket();
    m_InGameTicketsRepository->OnTicketDrawn(winningTicket);

    auto flyThroughTicket = m_RandomTicketDraw.GetNextPresentableTicket();
    setCurrentFlyThroughTicket(flyThroughTicket);
    emit ticketFlyThroughRequested();
}

void SingleTicketDraw_ViewModel::onFlyUpTicketStateAchieved()
{
    auto flyThroughTicket = m_RandomTicketDraw.GetNextPresentableTicket();
    setCurrentFlyThroughTicket(flyThroughTicket);
    if (m_RandomTicketDraw.IsDone())
        emit ticketWinningPositionRequested();
    else
        emit ticketFlyThroughRequested();
}

void SingleTicketDraw_ViewModel::onWinningTicketStateAchieved()
{
    m_TicketDrawState = TicketDrawState::WinningTicketShown;
}

void SingleTicketDraw_ViewModel::onVanishedTicketStateAchieved()
{
    insertRow(0);
    emit dataChanged(createIndex(0,0), createIndex(rowCount(),0));

    onStartSingleTicketDraw();
    emit dataChanged(createIndex(0,0), createIndex(rowCount(),0));
}

void SingleTicketDraw_ViewModel::removeFromWinningTicketsHistory(int index)
{
    removeRow(index);
}

const QColor &SingleTicketDraw_ViewModel::flyingTicketBgColor() const
{
    if (!m_CurrentFlyThroughTicket) // dummy fallback
        return BlockColorsSet::GetBlockColor(0);
    else
        return BlockColorsSet::GetBlockColor( m_CurrentFlyThroughTicket->ColorsSetIndex() );
}

const QColor &SingleTicketDraw_ViewModel::flyingTicketTextColor() const
{
    if (!m_CurrentFlyThroughTicket) // dummy fallback
        return BlockColorsSet::GetTextColor(0);
    else
        return BlockColorsSet::GetTextColor( m_CurrentFlyThroughTicket->ColorsSetIndex() );
}

static QString dummyFlyingTicketBlockName("");
const QString &SingleTicketDraw_ViewModel::flyingTicketBlockName() const
{
    if (!m_CurrentFlyThroughTicket) // dummy fallback
        return dummyFlyingTicketBlockName;
    else
        return m_CurrentFlyThroughTicket->BlockName();
}

int SingleTicketDraw_ViewModel::flyingTicketId() const
{
    if (!m_CurrentFlyThroughTicket) // dummy fallback
        return 0;
    else
        return m_CurrentFlyThroughTicket->TicketNumber();
}

int SingleTicketDraw_ViewModel::animationDuration() const
{
    int decceleration = m_CurrentFlyThroughCounter - SingleTicketDraw_ViewModel::DefaultStepsCountOfDraw / 3;
    decceleration = std::max(0, decceleration);
    if (decceleration == 0)
        return SingleTicketDraw_ViewModel::FlyThroughAnimationDurationStartValue;
    else
        return static_cast<int>(SingleTicketDraw_ViewModel::FlyThroughAnimationDurationStartValue / pow(.95, decceleration));
}

QHash<int, QByteArray> SingleTicketDraw_ViewModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[BackgroundColorRole] = "backgroundColor";
    roles[TextColorRole] = "textColor";
    roles[NameRole] = "name";
    roles[TicketIdRole] = "ticketId";
    return roles;
}

bool SingleTicketDraw_ViewModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    m_WinningTicketsHistory.push_front(m_CurrentFlyThroughTicket);
    endInsertRows();

    return true;
}

bool SingleTicketDraw_ViewModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    while(count--)
    {
        auto itWinningTicketToHide = std::next(m_WinningTicketsHistory.begin(), row);
        auto itAfterWinningTicketToHide = std::next(itWinningTicketToHide);
        m_WinningTicketsHistory.erase(itWinningTicketToHide, itAfterWinningTicketToHide);
    }
    endRemoveRows();

    return true;
}

void SingleTicketDraw_ViewModel::setCurrentFlyThroughTicket(std::shared_ptr<Ticket> ticket)
{
    m_CurrentFlyThroughCounter++;
    emit animationDurationChanged();

    m_CurrentFlyThroughTicket = ticket;
    emit flyingTicketBgColorChanged();
    emit flyingTicketTextColorChanged();
    emit flyingTicketBlockNameChanged();
    emit flyingTicketIdChanged();
}

