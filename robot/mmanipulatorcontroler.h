#ifndef MMANIPULATORCONTROLER_H
#define MMANIPULATORCONTROLER_H

#include <QObject>
#include <QtWebChannel/QWebChannelAbstractTransport>

class mManipulatorControler : public QObject
{
    Q_OBJECT
public:
    explicit mManipulatorControler(QObject *parent = 0);


signals:

public slots:
};

#endif // MMANIPULATORCONTROLER_H
