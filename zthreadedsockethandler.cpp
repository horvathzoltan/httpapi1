#include "zthreadedsockethandler.h"
#include "zlog.h"
//#include "zrequest.h"
//#include "zactionhelper.h"
//#include "zresponse.h"
//#include <QUrlQuery>
#include <QThreadPool>
#include "zactiontask.h"

extern zLog zlog;

const QString zThreadedSocketHandler::LOGPATTERN = QStringLiteral("conn %1: %2");

zThreadedSocketHandler::zThreadedSocketHandler(QObject *parent) : QObject(parent)
{
    QThreadPool::globalInstance()->setMaxThreadCount(5);
}

// amikor a szerver elkap egy bejövő connectiont, csinál egy socketet
// a szervertől kapott descriptoral
void zThreadedSocketHandler::setSocket(qintptr descriptor)
{
    // make a new socket
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket->setSocketDescriptor(descriptor);

    //trace("created");
}


void zThreadedSocketHandler::setServer(zHTTPThreadedServer *s)
{
    this->server = s;
}

// asynchronous - runs separately from the thread we created
void zThreadedSocketHandler::connected()
{
    //trace(QStringLiteral("connected"));
}

// asynchronous
void zThreadedSocketHandler::disconnected()
{
    //trace(QStringLiteral("disconnected"));
}

// ez itt mind a fő szálon történik
// de mi a taskot a poolból vesszük és haszáljuk fel

void zThreadedSocketHandler::readyRead()
{
    /*
     *
I've tried this myself now with a very small example code and it works as it should be!? here my code:
@
QTcpServer *server = new QTcpServer;
connect(server, &QTcpServer::newConnection, [=]{
QTcpSocket *client = server->nextPendingConnection();
qDebug() << client->peerAddress() << client->peerPort();
connect(client, &QTcpSocket::readyRead, [=]{
qDebug() << client->bytesAvailable() << client->readAll();
});
});
qDebug() << server->listen(QHostAddress::LocalHost, 80);
@
I just print everything to the debug console.
example output send from chrome/postman extension as post data
@
*/
    QByteArray ba;
//    ba = socket->readAll();

    while(socket->bytesAvailable()) {
        ba.append(socket->readAll());
        }

    // ha request jön, fejéccel, meg minden, akkor kell új request, ha nem, akkor valószínűleg
    // content jön egy korábbi actionhöz vagy requesthez
    // nemmellesleg a contetnt jöhet egyben is
    zRequest r(ba);

//    QByteArray ba2;

//    while(socket->bytesAvailable()) {
//        ba2.append(socket->readAll());
//        }

    if(r.method == zRequest::Method::POST){
        trace(zRequest::Method::POST);
    }
    trace(r.toString());

    //auto action = zActionHelper::find(this->server->actions, r);

    auto action = server->action(r);

    if(action != nullptr)
    {
        // kiolvassuk a hívást, megtaláljuk, melyik actiont kell a tasknak átadni
        // a task csak az actiont végrehajtja és az eredményt közli
        // ez bekerül egy response-ba, amit a TaskResult visszaad

        // ha az action minden byte-ja megjött, akkor ok, ha nem, akkor egy újabb readtoready-ben fog jönni hozzá.
        // ha ok, akkor lesz task, ha nem, akkor wait lesz.

        zActionTask *mytask = new zActionTask();
        //QString q_str = r.url.query();

        mytask->setActionFn(action->fn, r.urlparams, r.content);
        mytask->setAutoDelete(true);

        // sorba állítjuk
        connect(mytask, SIGNAL(Result(zActionResult)), this, SLOT(TaskResult(zActionResult)), Qt::QueuedConnection);

        QThreadPool::globalInstance()->start(mytask);
    }
    else
    {
        zResponse rs;

        rs.setStatus(zResponse::statusCode::NotFound);
        rs.addHeaderField(zResponse::headerField::Server, server->serverName());
        sendResponse(rs);

    }
}

// miután a task elkészült
// az eredményt válasszá alakítjuk
void zThreadedSocketHandler::TaskResult(zActionResult e)
{
    zResponse rs;

    rs.setStatus(zResponse::statusCode::OK);
    rs.addBody(e);
    rs.addHeaderField(zResponse::headerField::Server, server->serverName());

    sendResponse(rs);
}


// az api válaszát közöljük a klienssel
void zThreadedSocketHandler::sendResponse(zResponse rs){

    trace(rs.getStatus());
    socket->write(rs.toByteArray());
    socket->disconnectFromHost();
}

void zThreadedSocketHandler::trace(QString p1)
{
    zlog.trace(LOGPATTERN.arg(socket->socketDescriptor()).arg(p1));
}
