#include "globals.h"
#include "mainwindow.h"
#include "zhttpthreadedserver.h"
#include "zlog.h"
#include "zrequest.h"
#include "zaction.h"
#include "dowork.h"
#include "zstringhelper.h"
#include <QApplication>
#include <signal.h>
#ifdef RPI
#include <wiringPi.h>
#endif

int i;
zLog zlog;
zHTTPThreadedServer server(QStringLiteral("piapi2"));

void handleShutDownSignal( int signalId )
{
    server.stop();
    zlog.trace(QStringLiteral("EXIT: %1").arg(signalId));
    QApplication::exit(0);
}

void setShutDownSignal( int signalId )
{
struct sigaction sa;
sa.sa_flags = 0;
sigemptyset(&sa.sa_mask);
sa.sa_handler = handleShutDownSignal;
if (sigaction(signalId, &sa, nullptr) == -1)
{
perror("setting up termination signal");
exit(1);
}
}


int main(int argc, char *argv[])
{    
    setShutDownSignal(SIGINT); // shut down on ctrl-c
    setShutDownSignal(SIGTERM); // shut down on killall

#ifdef RPI
    wiringPiSetup();
    pinMode (1, OUTPUT);
    digitalWrite (1, HIGH);
#endif

    QApplication a(argc, argv);       

    // ha lenne ablak, és lenne listWidget amibe a logger logolhat
    // konzolalkalmazásnál ez a standard err
     MainWindow w;
     w.show();
     zlog.init(w.logListWidget());

    zlog.ok("started");

    qRegisterMetaType<zActionResult>(qPrintable(nameof(zActionResult)));

    //zHTTPThreadedServer server(QStringLiteral("piapi1"));
    server.setHostAddress(QHostAddress::Any, 8080);
    server.addAction(zRequest::Method::GET, "/dolgoz1", &doWork::dolgoz1);
    server.addAction(zRequest::Method::GET, "/dolgoz2", &doWork::dolgoz2);        
    server.addAction(zRequest::Method::GET, "/getNext", &doWork::getNextNumber);
    server.addAction(zRequest::Method::GET, "/styro1/translation/getheaderuc", &doWork::getHeaderUCtr1);
    // styro1
    server.addAction(zRequest::Method::GET, "/styro1/translation/getmainpagetr1", &doWork::getMainPagetr1);
    //testconnection
    server.addAction(zRequest::Method::GET, "/styro1/login/active", &doWork::testconnection);
    server.addAction(zRequest::Method::POST, "/styro1/login/login", &doWork::login);
    server.start();

    return a.exec();
}




