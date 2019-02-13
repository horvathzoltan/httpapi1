#include "zresponse.h"
#include "zlog.h"

extern zLog zlog;

const QString zResponse::keyValuePattern QStringLiteral("%1: %2\r\n");
const QString zResponse::statusLinePattern QStringLiteral("%1 %2 %3\r\n");
const QString zResponse::HTTPversion QStringLiteral("HTTP/1.1");

const QString zResponse::headerField::ContentType = QStringLiteral("Content-Type");
const QString zResponse::headerField::ContentLength = QStringLiteral("Content-Length");
const QString zResponse::headerField::Connection = QStringLiteral("Connection");
const QString zResponse::headerField::Server = QStringLiteral("Server");

//Informational
const int zResponse::statusCode::Continue=100;
const int zResponse::statusCode::SwitchingProtocols=101;
//Successful
const int zResponse::statusCode::OK=200;
const int zResponse::statusCode::Created=201;
const int zResponse::statusCode::Accepted=202;
const int zResponse::statusCode::NonAuthoritativeInformation=203;
const int zResponse::statusCode::NoContent=204;
const int zResponse::statusCode::ResetContent=205;
const int zResponse::statusCode::PartialContent=206;
//Redirection;
const int zResponse::statusCode::MultipleChoices=300;
const int zResponse::statusCode::MovedPermanently=301;
const int zResponse::statusCode::Found=302;
const int zResponse::statusCode::SeeOther=303;
const int zResponse::statusCode::NotModified=304;
const int zResponse::statusCode::UseProxy=305;
const int zResponse::statusCode::TemporaryRedirect=307;
//Client Error
const int zResponse::statusCode::BadRequest=400;
const int zResponse::statusCode::Unauthorized=401;
const int zResponse::statusCode::PaymentRequired=402;
const int zResponse::statusCode::Forbidden=403;
const int zResponse::statusCode::NotFound=404;
const int zResponse::statusCode::MethodNotAllowed=405;
const int zResponse::statusCode::NotAcceptable=406;
const int zResponse::statusCode::ProxyAuthenticationRequired=407;
const int zResponse::statusCode::RequestTimeout=408;
const int zResponse::statusCode::Conflict=409;
const int zResponse::statusCode::Gone=410;
const int zResponse::statusCode::LengthRequired=411;
const int zResponse::statusCode::PreconditionFailed=412;
const int zResponse::statusCode::RequestEntityTooLarge=413;
const int zResponse::statusCode::RequestURItooLarge=414;
const int zResponse::statusCode::UnsupportedMediaType=415;
const int zResponse::statusCode::RequestedRangeNotSatisfiable=416;
const int zResponse::statusCode::ExpectationFailed=417;
//Server Error
const int zResponse::statusCode::InternalServerError=500;
const int zResponse::statusCode::NotImplemented=501;
const int zResponse::statusCode::BadGateway=502;
const int zResponse::statusCode::ServiceUnavailable=503;
const int zResponse::statusCode::GatewayTimeout=504;
const int zResponse::statusCode::HTTPversionNotSupported=505;


const QMap<int, QString> zResponse::statuses {
    {101, QStringLiteral("Switching Protocols")},

    {zResponse::statusCode::OK, QStringLiteral("OK")},
    {zResponse::statusCode::Created, QStringLiteral("Created")},
    {202, QStringLiteral("Accepted")},
    {203, QStringLiteral("Non-Authoritative Information")},
    {204, QStringLiteral("No Content")},
    {205, QStringLiteral("Reset Content")},
    {206, QStringLiteral("Partial Content")},
    {300, QStringLiteral("Multiple Choices")},
    {301, QStringLiteral("Moved Permanently")},
    {302, QStringLiteral("Found")},
    {303, QStringLiteral("See Other")},
    {304, QStringLiteral("Not Modified")},
    {305, QStringLiteral("Use Proxy")},
    {307, QStringLiteral("Temporary Redirect")},

    {zResponse::statusCode::BadRequest, QStringLiteral("Bad Request")},
    {401, QStringLiteral("Unauthorized")},
    {402, QStringLiteral("Payment Required")},
    {403, QStringLiteral("Forbidden")},
    {zResponse::statusCode::NotFound, QStringLiteral("Not Found")},
    {zResponse::statusCode::MethodNotAllowed, QStringLiteral("Method Not Allowed")},
    {406, QStringLiteral("Not Acceptable")},
    {407, QStringLiteral("Proxy Authentication Required")},
    {408, QStringLiteral("Request Time-out")},
    {409, QStringLiteral("Conflict")},
    {410, QStringLiteral("Gone")},
    {411, QStringLiteral("Length Required")},
    {412, QStringLiteral("Precondition Failed")},
    {413, QStringLiteral("Request Entity Too Large")},
    {414, QStringLiteral("Request-URI Too Large")},
    {415, QStringLiteral("Unsupported Media Type")},
    {416, QStringLiteral("Requested range not satisfiable")},
    {417, QStringLiteral("Expectation Failed")},

    {zResponse::statusCode::InternalServerError, QStringLiteral("Internal Server Error")},
    {zResponse::statusCode::NotImplemented, QStringLiteral("Not Implemented")},
    {502, QStringLiteral("Bad Gateway")},
    {zResponse::statusCode::ServiceUnavailable, QStringLiteral("Service Unavailable")},
    {504, QStringLiteral("Gateway Time-out")},
    {505, QStringLiteral("HTTP Version not supported")},
};

zResponse::zResponse()
{

}


//Response::Response(int s,const QByteArray& b)
//{
//    this->statusCode = s;
//    this->body =b;
//}

void zResponse::setStatus(int s)
{
    this->statusCode = s;
}


void zResponse::addBody(const QByteArray& b)
{    
    this->body.append(b);
}

void zResponse::addHeaderField(const QString& k, const QString& v)
{
    if(!headerFields.contains(k))
    {
        headerFields.insert(k, v);
    }
    else
    {
        zlog.error(QStringLiteral("headerField already exist: %1").arg(k));
    }
}

QByteArray zResponse::toByteArray()
{
    QByteArray b;

    b.append(statusLinePattern.arg(HTTPversion).arg(statusCode).arg(zResponse::statusPhrase(statusCode)).toUtf8());

    if(!body.isEmpty())
    {
        this->addHeaderField(zResponse::headerField::ContentLength, QString::number(body.length()));
    }

    for(auto i = headerFields.begin();i!=headerFields.end();i++)
    {
        b.append(keyValuePattern.arg(i.key(), i.value()).toUtf8());
    }

    b.append("\r\n");
    if(!body.isEmpty())
    {
        b.append(body);
    }
    return b;
}




QString zResponse::statusPhrase(int s)
{
    return statuses.value(s, QStringLiteral("Unknown"));
}

void zResponse::addBody(const zActionResult& a){
    addBody(a.content);
    addHeaderField(zResponse::headerField::ContentType, a.type+" charset=utf-8");
}


QString zResponse::getStatus()
{
    return QStringLiteral("%1: %2").arg(this->statusCode).arg(this->statusPhrase(this->statusCode));
}
