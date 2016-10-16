#include "DynamicTranslation.h"
#include <QSettings>

static const QString LanguageSetting = "general/language";

DynamicTranslation::DynamicTranslation(QApplication &app, QObject *parent)
    : QObject(parent)
    , m_QApplication(app)
{
    m_TranslationLanguagesModel.reset(new TranslationLanguages_ViewModel());

    m_TranslatorHU = new QTranslator(this);
    m_TranslatorSK = new QTranslator(this);

    QSettings settings;
    QString language = settings.value(LanguageSetting, "hu").toString();
    int languageIndex = m_TranslationLanguagesModel->indexOfLanguage(language);
    selectLanguage(languageIndex);
}

DynamicTranslation::~DynamicTranslation()
{
    QSettings settings;
    settings.setValue(LanguageSetting, m_CurrentLocaleId);
}

void DynamicTranslation::selectLanguage(int languageIndex)
{
    QString language = m_TranslationLanguagesModel->languageOfIndex(languageIndex);
    if(language.toLower() == "hu")
    {
        m_TranslatorHU->load("tombola_hu", ".");
        m_QApplication.installTranslator(m_TranslatorHU);
    }
    else if(language.toLower() == "sk")
    {
        m_TranslatorSK->load("tombola_sk", ".");
        m_QApplication.installTranslator(m_TranslatorSK);
    }
    else
    {
        m_QApplication.removeTranslator(m_TranslatorHU);
        m_QApplication.removeTranslator(m_TranslatorSK);
    }

    m_CurrentLocaleId = language;

    emit languageChanged();
}

QAbstractListModel* DynamicTranslation::translationLanguages() const
{
    return m_TranslationLanguagesModel.get();
}

int DynamicTranslation::getSelectedLanguageIndex() const
{
    return m_TranslationLanguagesModel->indexOfLanguage(m_CurrentLocaleId);
}
