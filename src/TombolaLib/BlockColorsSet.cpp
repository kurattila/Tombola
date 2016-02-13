#include "BlockColorsSet.h"

std::vector<QColor> BlockColorsSet::BlockColors;
std::vector<QColor> BlockColorsSet::TextColors;
std::vector<QColor> BlockColorsSet::UnsoldTicketColors;
std::vector<QString> BlockColorsSet::ColorNames;


void BlockColorsSet::ensureInitDone()
{
    if (!BlockColorsSet::BlockColors.empty())
        return;

    QColor c;

    c.setNamedColor("#ffffff");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#000"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#000"));
    BlockColorsSet::ColorNames.push_back("White");

    c.setNamedColor("#000000");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#fff"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Black");

    c.setNamedColor("#7f7f7f");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#fff"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Gray");

    c.setNamedColor("#880015");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#fff"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Purple");

    c.setNamedColor("#ed1c24");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#fff"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Red");

    c.setNamedColor("#ff7f27");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#000"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Orange");

    c.setNamedColor("#fff200");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#000"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#888"));
    BlockColorsSet::ColorNames.push_back("Yellow");

    c.setNamedColor("#22b14c");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#000"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Green");

    c.setNamedColor("#00a2e8");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#000"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Cyan");

    c.setNamedColor("#3f48cc");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#fff"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Blue");

    c.setNamedColor("#a349a4");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#fff"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Magenta");

    c.setNamedColor("#b97a57");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#fff"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Brown");
}

int BlockColorsSet::GetColorsCount()
{
    ensureInitDone();
    return static_cast<int>(BlockColorsSet::BlockColors.size());
}

const QColor &BlockColorsSet::GetBlockColor(int colorSetIndex)
{
    ensureInitDone();
    return BlockColorsSet::BlockColors[colorSetIndex];
}

const QColor &BlockColorsSet::GetTextColor(int colorSetIndex)
{
    ensureInitDone();
    return BlockColorsSet::TextColors[colorSetIndex];
}

const QColor &BlockColorsSet::GetUnsoldTicketColor(int colorSetIndex)
{
    ensureInitDone();
    return BlockColorsSet::UnsoldTicketColors[colorSetIndex];
}

const QString &BlockColorsSet::GetColorName(int colorSetIndex)
{
    ensureInitDone();
    return BlockColorsSet::ColorNames[colorSetIndex];
}
