#include "DynamicTranslation.h"

DynamicTranslation::DynamicTranslation(QApplication &app, QObject *parent)
    : QObject(parent)
    , m_QApplication(app)
{
    m_TranslatorHU = new QTranslator(this);
    m_TranslatorSK = new QTranslator(this);
}

void DynamicTranslation::selectLanguage(QString language)
{
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
    else if(language.toLower() == "en")
    {
        m_QApplication.removeTranslator(m_TranslatorHU);
        m_QApplication.removeTranslator(m_TranslatorSK);
    }

    emit languageChanged();
}
