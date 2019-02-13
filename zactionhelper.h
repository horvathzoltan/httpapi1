#ifndef ACTIONHELPER_H
#define ACTIONHELPER_H

#include "zaction.h"

class zActionHelper
{
public:
    zActionHelper();

    static const zAction* find(const QList<zAction>& actions, QString r, QString p);
    static const zAction* find(const QList<zAction>& actions, const zRequest& r);
    static bool contains(const QList<zAction>& actions, QString r, QString p);
};

#endif // ACTIONHELPER_H
