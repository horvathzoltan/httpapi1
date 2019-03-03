#ifndef MYHTTPTHREADSERVER
#define MYHTTPTHREADSERVER

#include <iostream>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include "zaction.h"
#include "zrequest.h"

class zHTTPThreadedServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit zHTTPThreadedServer(const QString&, QObject *parent = nullptr);
    void setHostAddress(const QHostAddress &, quint16);
    void start();
    void stop();
    void addAction(QString, QString, zactionFn);

    const zAction* action(zRequest r);
    const QString serverName(void) const { return _serverName; }
    //QList<zRequest> pending_requests;
    QMap<qintptr, zRequest> pending_requests;
protected:
    void incomingConnection(qintptr socketDescriptor);

    QHostAddress host;
    quint16 port;
    QString _serverName;
    QList<zAction> actions;

};
#endif // MYHTTPTHREADSERVER
