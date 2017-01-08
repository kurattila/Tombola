#ifndef BLOCKCOLORSSET_H
#define BLOCKCOLORSSET_H

#include "tombolalib_global.h"

#include <QColor>
#include <QString>
#include <vector>

class TOMBOLALIBSHARED_EXPORT BlockColorsSet
{
    static std::vector<QColor> BlockColors;
    static std::vector<QColor> TextColors;
    static std::vector<QColor> UnsoldTicketColors;
    static std::vector<QString> ColorNames;
    static std::vector<QString> ShaderBlendModes;

    static void ensureInitDone();

public:
    static int GetColorsCount();
    static const QColor& GetBlockColor(int colorSetIndex);
    static const QColor& GetTextColor(int colorSetIndex);
    static const QColor& GetUnsoldTicketColor(int colorSetIndex);
    static const QString& GetColorName(int colorSetIndex);
    static const QString& GetShaderBlendMode(int colorSetIndex);
};

#endif // BLOCKCOLORSSET_H
