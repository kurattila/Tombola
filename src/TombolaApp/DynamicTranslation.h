#ifndef DYNAMICTRANSLATION_H
#define DYNAMICTRANSLATION_H

#include <QObject>
#include <QTranslator>
#include <QApplication>
#include <memory>
#include "TranslationLanguages_ViewModel.h"

class DynamicTranslation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString emptyString READ getEmptyString NOTIFY languageChanged)

    Q_PROPERTY(int selectedLanguageIndex READ getSelectedLanguageIndex NOTIFY languageChanged)

    QApplication& m_QApplication;

    QTranslator *m_TranslatorHU;
    QTranslator *m_TranslatorSK;

    QString m_CurrentLocaleId;
    std::unique_ptr<TranslationLanguages_ViewModel> m_TranslationLanguagesModel;

    int getSelectedLanguageIndex() const;
    QAbstractListModel* translationLanguages() const;

public:
    explicit DynamicTranslation(QApplication& app, QObject *parent = 0);
    ~DynamicTranslation();

    Q_INVOKABLE void selectLanguage(int languageIndex);
    Q_PROPERTY(QAbstractListModel* translationLanguages READ translationLanguages NOTIFY translationLanguagesChanged)

    QString getEmptyString() const { return ""; }

signals:
    void languageChanged();
    void translationLanguagesChanged(); // declare this just to omit warning with 'translationLanguages': "depends on non-notifiable property"
};

#endif // DYNAMICTRANSLATION_H
