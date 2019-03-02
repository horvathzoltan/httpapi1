#ifndef DOWORK_H
#define DOWORK_H

#include "zaction.h"

class doWork
{
public:
    doWork();

    static zActionResult dolgoz1(const QUrlQuery& param, const QByteArray& content);
    static zActionResult dolgoz2(const QUrlQuery& param, const QByteArray& content);
    static zActionResult getNextNumber(const QUrlQuery& param, const QByteArray& content);

    static zActionResult getHeaderUCtr1(const QUrlQuery& param, const QByteArray& content);
    static zActionResult getMainPagetr1(const QUrlQuery& param, const QByteArray& content);


    static zActionResult testconnection(const QUrlQuery& param, const QByteArray& content);
    static zActionResult login(const QUrlQuery& param, const QByteArray& content);
};

#endif // DOWORK_H
