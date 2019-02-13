#include "zrequest.h"
#include "zlog.h"

extern zLog zlog;

/*
https://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html
*/

const QString zRequest::Method::OPTIONS = QStringLiteral("OPTIONS");
const QString zRequest::Method::GET = QStringLiteral("GET");
const QString zRequest::Method::HEAD = QStringLiteral("HEAD");
const QString zRequest::Method::POST = QStringLiteral("POST");
const QString zRequest::Method::PUT = QStringLiteral("PUT");
const QString zRequest::Method::DELETE = QStringLiteral("DELETE");
const QString zRequest::Method::TRACE = QStringLiteral("TRACE");
const QString zRequest::Method::CONNECT = QStringLiteral("CONNECT");

zRequest::zRequest()
{

}

zRequest::zRequest(QString r)
{
    auto lines = r.split("\r\n");
    QString requestLine = lines[0];

    auto rd = r;

    if(!requestLine.isEmpty())
    {
        auto requestLineParts = requestLine.split(' ');

        if(requestLineParts.count()==3)
        {
            this->method = requestLineParts[0];
            this->URI = requestLineParts[1];
            this->HTTPversion = requestLineParts[2];

            if(!URI.isEmpty())
            {
                this->url = QUrl(URI);                
                this->urlparams = QUrlQuery(url);
            }
        }
        else
        {
            zlog.error("requestLine inappropriate format");
        }

        if(lines.count()>1)
        {
            for(int i = 1;i<lines.count();i++)
            {
                QString line = lines[i];
                if(line.isEmpty()) continue;
                auto ix = line.indexOf(':');

                if(ix>-1)//és ha 0, akkor nincs kulcs...
                {
                    if(ix>0)
                    {
                        this->headerFields.insert(line.left(ix),line.mid(ix+1));
                    }
                    else
                    {
                        zlog.error(QStringLiteral("there is no key: %1").arg(line));
                    }

                }
                else
                {
                    zlog.error("headerFieldLine inappropriate format");
                }
            }
        }
    }
}

QString zRequest::toString()
{
    auto e = QStringLiteral("%1 %2 %3").arg(this->method, this->URI, this->url.toString());
    return e;
}




