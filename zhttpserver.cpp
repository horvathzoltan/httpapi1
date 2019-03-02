#include "zhttpserver.h"
#include "zlog.h"

#include "zresponse.h"
#include "zactionhelper.h"
#include <QTextCodec>


extern zLog zlog;

zHTTPserver::zHTTPserver(const QString& name, const QHostAddress& host, quint16 port, QObject *parent) : QObject(parent)
    {
    server = new QTcpServer(this);
    this->serverName = name;
    connect(server, SIGNAL(newConnection()),this, SLOT(incomingConnection()));
    if(!server->listen(host,port))
    {
        zlog.trace("Web server could not start");
    }
    else
    {
        zlog.trace("Web server is waiting for a connection on port 8080");
    }
}

void zHTTPserver::incomingConnection()
    {
    socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(txRx()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(closingClient()));
}

const QTextCodec* zHTTPserver::UTF8codec = QTextCodec::codecForName("UTF-8");

void zHTTPserver::txRx()
    {
    char webBrowerRXData[1000];
    qint64 sv=socket->read(webBrowerRXData,1000);
    //zlog.trace("reading web browser data");
    QString a(webBrowerRXData);

    zRequest r(a);

    zlog.trace(r.toString());

    //auto action = zActionHelper::find(this->actions, r);
    auto ac = action(r);

    zResponse rs;
    if(ac != nullptr)
    {
        rs.setStatus(zResponse::statusCode::OK);
        //QString q = r.url.query();               
        auto eredmeny = ac->fn(r.urlparams, r.content);
        rs.addBody(eredmeny);
    }
    else
    {
        rs.setStatus(zResponse::statusCode::NotFound);
    }   

    rs.addHeaderField(zResponse::headerField::Server, this->serverName);
    socket->write(rs.toByteArray());
    socket->disconnectFromHost();
}

void zHTTPserver::closingClient()
    {
        socket->deleteLater();
}

zHTTPserver::~zHTTPserver()
    {
    zlog.trace("closing socket");
    socket->close();
}

void zHTTPserver::addAction(QString m, QString p, zactionFn fn)
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

const zAction* zHTTPserver::action(zRequest r)
{
    return zActionHelper::find(actions, r);
}
