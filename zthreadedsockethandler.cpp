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
    socket->setSocketDescriptor(descriptor);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    trace("socket created:"+QString::number(descriptor));
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
    QByteArray ba;
    auto descriptor = socket->socketDescriptor();

    ba = socket->readAll();

    zRequest r(ba);

    trace(r.toString());

    //const zAction* action = nullptr;

    if(r.status==1)
    {
        if(r.isCompleted())
        {
            auto action = server->action(r);
            if(action !=nullptr)
            {
                startTask(r, *action);
            }
            else
            {
                notFound();
            }
        }
        else
        {
            server->pending_requests[descriptor]=r;
        }
    }
    else
    {
        if(server->pending_requests.contains(descriptor))
        {
            auto r2 = server->pending_requests[descriptor];
            r2.content+=ba;

            if(r2.isCompleted())
            {
                server->pending_requests.remove(descriptor);
                auto action = server->action(r2);
                if(action != nullptr)
                {
                    startTask(r2, *action);
                }
                else
                {
                    notFound();
                }
            }
        }
        else
        {
            zResponse rs;
            rs.setStatus(zResponse::statusCode::OK);
            rs.addHeaderField(zResponse::headerField::Server, server->serverName());
            rs.addBody("");

            socket->write(rs.toByteArray());
            socket->close();
            socket->disconnectFromHost();

        }
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

void zThreadedSocketHandler::startTask(const zRequest& r, const zAction action){
    zlog.trace(QStringLiteral("TASK(%1)").arg(r.instance));
    zActionTask *mytask = new zActionTask();
    //QString q_str = r.url.query();

    mytask->setActionFn(action.fn, r.urlparams, r.content);
    mytask->setAutoDelete(true);

    // sorba állítjuk
    connect(mytask, SIGNAL(Result(zActionResult)), this, SLOT(TaskResult(zActionResult)), Qt::QueuedConnection);

    QThreadPool::globalInstance()->start(mytask);
}

void zThreadedSocketHandler::notFound()
{
    zResponse rs;

    rs.setStatus(zResponse::statusCode::NotFound);
    rs.addHeaderField(zResponse::headerField::Server, server->serverName());
    sendResponse(rs);
}
