#ifndef ZLOG_H
#define ZLOG_H

//#include <QString>
#include <QListWidget>
//#include <QTabWidget>
//#include <zstringhelper.h>

class zLog
{
private:
    QListWidget *widget;
    //QTabWidget *tabwidget;
    //int tabindex;
    static void dialog(const QString&, int);
    void log(const QString&, int);
    enum ErrLevels:int {OK, ERROR, WARNING, TRACE, MESSAGE};

public:
    //zLog();
    //~zLog();

    void init(QListWidget*);

    /*message*/
    static void dialogMessage(const QString& str);
    //void message(const char*);
    void message(const QString&);
    //void message(const QList<QString>&);

    /*ok*/
    static void dialogOk(const QString& str);
    //void ok(const char*);
    void ok(const QString&);
    //void ok(const QList<QString>&);

    /*warning*/
    static void dialogWarning(const QString& str);
    //void warning(const char*);
    void warning(const QString&);
    //void warning(const QList<QString>&);

    /*error*/
    static void dialogError(const QString& str);
    //void error(const char*);
    void error(const QString&);
    void error(const QString&, const QString&);
    //void error(const QList<QString>&);

    /*trace*/
    static void dialogTrace(const QString& str);
    //void trace(const char*);
    void trace(const QString&);
   // void trace(const char*, const QString&);
    void trace(const QString&, const QString&);
    //void trace(const QList<QString>&);
};

#endif // ZLOG_H
