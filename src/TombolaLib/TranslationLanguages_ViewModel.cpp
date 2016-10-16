#include "TranslationLanguages_ViewModel.h"

TranslationLanguages_ViewModel::TranslationLanguages_ViewModel()
{
    LanguageEntry hu = { "HU", "Magyar",    "Images/Flag_HU.png" };
    LanguageEntry sk = { "SK", "Slovensky", "Images/Flag_SK.png" };
    LanguageEntry en = { "EN", "English",   "Images/Flag_EN.png" };
    m_AllLanguages.push_back(hu);
    m_AllLanguages.push_back(sk);
    m_AllLanguages.push_back(en);
}

int TranslationLanguages_ViewModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_AllLanguages.size();
}

QVariant TranslationLanguages_ViewModel::data(const QModelIndex& index, int role) const
{
    const LanguageEntry& langEntry = m_AllLanguages.at(index.row());
    switch (role)
    {
    case DescriptionRole:
        return langEntry.Description;
    }

    return QVariant();
}

int TranslationLanguages_ViewModel::indexOfLanguage(const QString& localeId) const
{
    auto itFound = std::find_if(
                  std::begin(m_AllLanguages)
                , std::end(m_AllLanguages),
                [this, &localeId](const LanguageEntry& langEntry)
    {
        return (langEntry.LocaleId == localeId);
    });
    if (itFound != std::end(m_AllLanguages))
        return std::distance(std::begin(m_AllLanguages), itFound);
    else
        return -1;
}

QString TranslationLanguages_ViewModel::languageOfIndex(int languageIndex) const
{
    if (languageIndex >= static_cast<int>(m_AllLanguages.size()))
        return "";
    else
        return m_AllLanguages[languageIndex].LocaleId;
}

QHash<int, QByteArray> TranslationLanguages_ViewModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DescriptionRole] = "description";
    return roles;
}
