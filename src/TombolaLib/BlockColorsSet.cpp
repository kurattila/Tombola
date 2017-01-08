#include "BlockColorsSet.h"

std::vector<QColor> BlockColorsSet::BlockColors;
std::vector<QColor> BlockColorsSet::TextColors;
std::vector<QColor> BlockColorsSet::UnsoldTicketColors;
std::vector<QString> BlockColorsSet::ColorNames;
std::vector<QString> BlockColorsSet::ShaderBlendModes;


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
    BlockColorsSet::ShaderBlendModes.push_back("average");

    c.setNamedColor("#000000");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#fff"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Black");
    BlockColorsSet::ShaderBlendModes.push_back("average");

    c.setNamedColor("#7f7f7f");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#fff"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Gray");
    BlockColorsSet::ShaderBlendModes.push_back("softLight");

    c.setNamedColor("#880015");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#fff"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Purple");
    BlockColorsSet::ShaderBlendModes.push_back("average");

    c.setNamedColor("#ed1c24");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#fff"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Red");
    BlockColorsSet::ShaderBlendModes.push_back("average");

    c.setNamedColor("#ff7f27");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#000"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Orange");
    BlockColorsSet::ShaderBlendModes.push_back("softLight");

    c.setNamedColor("#fff200");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#000"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#888"));
    BlockColorsSet::ColorNames.push_back("Yellow");
    BlockColorsSet::ShaderBlendModes.push_back("average");

    c.setNamedColor("#22b14c");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#000"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Green");
    BlockColorsSet::ShaderBlendModes.push_back("softLight");

    c.setNamedColor("#00a2e8");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#000"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Cyan");
    BlockColorsSet::ShaderBlendModes.push_back("softLight");

    c.setNamedColor("#3f48cc");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#fff"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Blue");
    BlockColorsSet::ShaderBlendModes.push_back("softLight");

    c.setNamedColor("#a349a4");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#fff"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Magenta");
    BlockColorsSet::ShaderBlendModes.push_back("softLight");

    c.setNamedColor("#b97a57");
    BlockColorsSet::BlockColors.push_back(c);
    BlockColorsSet::TextColors.push_back(QColor("#fff"));
    BlockColorsSet::UnsoldTicketColors.push_back(QColor("#fff"));
    BlockColorsSet::ColorNames.push_back("Brown");
    BlockColorsSet::ShaderBlendModes.push_back("softLight");
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

const QString &BlockColorsSet::GetShaderBlendMode(int colorSetIndex)
{
    ensureInitDone();
    return BlockColorsSet::ShaderBlendModes[colorSetIndex];
}
