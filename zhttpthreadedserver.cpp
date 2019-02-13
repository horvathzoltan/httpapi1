#include "zhttpthreadedserver.h"
#include "zlog.h"
#include "zresponse.h"
#include "zactionhelper.h"
#include <QTextCodec>
#include "zthreadedsockethandler.h"


extern zLog zlog;

zHTTPThreadedServer::zHTTPThreadedServer(const QString& name, QObject *parent) : QTcpServer(parent)
{
    this->_serverName = name;
}

void zHTTPThreadedServer::setHostAddress(const QHostAddress& a, quint16 p)
{
    this->host = a;
    this->port = p;
}

void zHTTPThreadedServer::start()
{
    if(!listen(host,port))
    {
        zlog.trace("Web server could not start");
    }
    else
    {
        zlog.trace(QStringLiteral("Web server:%3 is waiting for a connection on port %1:%2").arg(host.toString()).arg(port).arg(_serverName));
    }
}

void zHTTPThreadedServer::stop()
{
    this->close();
    zlog.trace(QStringLiteral("Web server:%1 is closed").arg(_serverName));
}

void zHTTPThreadedServer::incomingConnection(qintptr socketDescriptor)
{
    // Amikor bejön egy connection, csinálunk egy klienst , ami amúgy nem kliens, inkább socket handler
    zThreadedSocketHandler *client = new zThreadedSocketHandler(this);
    client->setSocket(socketDescriptor);
    //client->setActions(&this->actions);
    client->setServer(this);
}

void zHTTPThreadedServer::addAction(QString m, QString p, zactionFn fn)
{
    if(m.isEmpty()) return;
    if(p.isEmpty()) return;
    if(fn == nullptr) return;

    bool isContains = zActionHelper::contains(this->actions, m, p);

    if(isContains)
    {
        zlog.error(QStringLiteral("action already added: %1 %2").arg(m, p));
    }
    else
    {
        zAction a(m, p, fn);
        this->actions.append(a);
    }
}

const zAction* zHTTPThreadedServer::action(zRequest r)
{
    return zActionHelper::find(actions, r);
}

