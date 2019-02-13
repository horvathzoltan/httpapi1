#ifndef MYCLIENT_H
#define MYCLIENT_H

//#include <QObject>
//#include <QTcpSocket>
//#include <QDebug>
//#include <QThreadPool>
//#include "zactiontask.h"
#include "zaction.h"
#include "zhttpthreadedserver.h"
#include "zresponse.h"

class zThreadedSocketHandler : public QObject
{
    Q_OBJECT
public:
    explicit zThreadedSocketHandler(QObject *parent = nullptr);
    void setSocket(qintptr Descriptor);
    void setServer(zHTTPThreadedServer *s);
    void trace(QString);
//signals:

public slots:
    void connected();
    void disconnected();
    void readyRead();

    // make the server fully ascynchronous
    // by doing time consuming task
    void TaskResult(zActionResult e);

private:
    static const QString LOGPATTERN;

    QTcpSocket *socket;
    zHTTPThreadedServer *server;

    void sendResponse(zResponse e);
};

#endif // MYCLIENT_H
