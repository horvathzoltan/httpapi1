#include "zaction.h"

const QString zAction::contentType::HTML = QStringLiteral("text/html");
const QString zAction::contentType::TEXT = QStringLiteral("text/plain");
const QString zAction::contentType::JSON = QStringLiteral("application/json");

zAction::zAction()
{

}

zAction::zAction(QString m, QString p, zactionFn fn)
{
    this->method = m;
    this->path = p;
    this->fn = fn;
}


