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

int zRequest::instance_counter=0;

zRequest::zRequest()
{
    this->instance= instance_counter++;
    //auto a  = this->toString();
    zlog.trace(QStringLiteral("CONSTRUCT(%1)").arg(instance));//.arg(a)
}

zRequest::~zRequest()
{
    //auto a  = this->toString();
    zlog.trace(QStringLiteral("DESTRUCT(%1)").arg(instance));//.arg(a)
}

zRequest::zRequest(QString r)
{
    this->instance= instance_counter++;
    //auto a  = this->toString();
    zlog.trace(QStringLiteral("CONSTRUCT(%1)2").arg(instance));//.arg(a)
    //if(r.length()<8) {this->status=0; return;}
    this->status=1;
    this->content_length=-1;

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
                this->content = lines.last().toUtf8();
            }            
        }
        else
        {
            zlog.error("requestLine inappropriate format");
            this->status=0;
            return;
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
                        //this->status=0;
                    }

                }
                else
                {
                    zlog.error("headerFieldLine inappropriate format");
                    //this->status=0;
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

bool zRequest::isCompleted(){
    auto hasBody = this->headerFields.contains("Content-Length");
    if(hasBody)// && (r.content.isEmpty() || r.content.length()<))
    {
        int l = this->headerFields["Content-Length"].toInt();
        if(this->content.length()<l)
        {
            //részleges request - csak a fej van meg
            zlog.trace(QStringLiteral("partially %1 , %2").arg(l).arg(this->content.length()));

            return false;
        }
    }
    return true;
}



