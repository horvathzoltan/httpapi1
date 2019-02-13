#ifndef MYHTTPSERVER
#define MYHTTPSERVER

#include <iostream>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include "zaction.h"
#include "zrequest.h"

class zHTTPserver : public QObject
{
    Q_OBJECT
public:
    explicit zHTTPserver(const QString&, const QHostAddress &, quint16, QObject *parent = nullptr);
    ~zHTTPserver();
    void addAction(QString, QString, zactionFn);

    QTcpSocket *socket ;
signals:
    void dataReceived(QByteArray);
public slots:
    void incomingConnection();
    void txRx();   
    void closingClient();        
private:

    qint64 bytesAvailable() const;
    QTcpServer *server;
    QList<zAction> actions;

    static const QTextCodec* UTF8codec;

    QString serverName;

    const zAction* action(zRequest r);
};
#endif // MYHTTPSERVER
