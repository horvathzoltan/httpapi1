#ifndef MYTASK_H
#define MYTASK_H

#include <QObject>
#include <QRunnable>
#include "zaction.h"

// A signal/slot használata miatt kell származtatni a QObjectből
// a task miatt pedig a QRunnable-ből is

class zActionTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    zActionTask();
    void setActionFn(const zactionFn& a, const QUrlQuery& param);
signals:
    // a fő szálat értesíti és adja át az eredményt
    void Result(zActionResult e);

protected:
    void run();
    zactionFn aFn;
    QUrlQuery aParam;
};

#endif // MYTASK_H
