#ifndef DOWORK_H
#define DOWORK_H

#include "zaction.h"

class doWork
{
public:
    doWork();

    static zActionResult dolgoz1(const QUrlQuery& param);
    static zActionResult dolgoz2(const QUrlQuery& param);
};

#endif // DOWORK_H
