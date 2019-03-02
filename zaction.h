#ifndef ACTION_H
#define ACTION_H

#include "zrequest.h"

// ez történik amikor egy kérés befut a szerverhez
// és a kérés methodhoz és url-hez van társítva egy függvény

struct zActionResult{
    QByteArray content;
    QString type;

    zActionResult(){}
    zActionResult(const QByteArray& b, const QString& t){
        this->content = b;
        this->type = t;
    }
};
//Q_DECLARE_METATYPE(zActionResult);

typedef zActionResult (*zactionFn)(const QUrlQuery&, const QByteArray&);

class zAction
{
public:
    struct contentType{
        static const QString HTML;
        static const QString TEXT;
        static const QString JSON;
    };

    zAction();

    zAction(QString, QString, zactionFn);

    QString method;
    QString path;
    zactionFn fn;
};



#endif // ACTION_H
