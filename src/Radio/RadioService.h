#ifndef RADIOSERVICE_H
#define RADIOSERVICE_H

#include <QObject>

class RadioService : public QObject
{
    Q_OBJECT
public:
    explicit RadioService(QObject *parent = nullptr);

signals:

};

#endif // RADIOSERVICE_H
