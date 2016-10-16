#ifndef TRANSLATIONLANGUAGES_VIEWMODEL_H
#define TRANSLATIONLANGUAGES_VIEWMODEL_H

#include "tombolalib_global.h"
#include <QAbstractListModel>

class TOMBOLALIBSHARED_EXPORT TranslationLanguages_ViewModel : public QAbstractListModel
{
public:
    TranslationLanguages_ViewModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    int indexOfLanguage(const QString& localeId) const;
    QString languageOfIndex(int languageIndex) const;

protected:
    enum TranslationLanguagesRoles
    {
        DescriptionRole = Qt::UserRole + 1
    };

    QHash<int, QByteArray> roleNames() const;

    struct LanguageEntry
    {
        QString LocaleId;
        QString Description;
        QString FlagImagePath;
    };

    std::vector<LanguageEntry> m_AllLanguages;
};

#endif // TRANSLATIONLANGUAGES_VIEWMODEL_H
