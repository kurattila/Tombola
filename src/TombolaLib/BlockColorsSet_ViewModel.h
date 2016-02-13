#ifndef BLOCKCOLORSSET_VIEWMODEL_H
#define BLOCKCOLORSSET_VIEWMODEL_H

#include "tombolalib_global.h"
#include <QAbstractListModel>

class TOMBOLALIBSHARED_EXPORT BlockColorsSet_ViewModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit BlockColorsSet_ViewModel(QObject *parent = 0);

    enum ColorsSetRoles
    {
        BackgroundColorRole = Qt::UserRole + 1
    };

    Q_PROPERTY(int colorsCount READ getColorsCount NOTIFY colorsCountChanged)

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

    int getColorsCount() const;

signals:
    void colorsCountChanged();

public slots:
};

#endif // BLOCKCOLORSSET_VIEWMODEL_H
