#ifndef DYNAMICTRANSLATION_H
#define DYNAMICTRANSLATION_H

#include <QObject>
#include <QTranslator>
#include <QApplication>

class DynamicTranslation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString emptyString READ getEmptyString NOTIFY languageChanged)

    QApplication& m_QApplication;

    QTranslator *m_TranslatorHU;
    QTranslator *m_TranslatorSK;

public:
    explicit DynamicTranslation(QApplication& app, QObject *parent = 0);

    Q_INVOKABLE void selectLanguage(QString language);
    QString getEmptyString() const { return ""; }

signals:
    void languageChanged();
};

#endif // DYNAMICTRANSLATION_H
