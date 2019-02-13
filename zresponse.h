#ifndef RESPONSE_H
#define RESPONSE_H

#include <QMap>
#include "zaction.h"

class zResponse
{
public:
    zResponse();

    struct headerField
    {
       static const QString ContentType;
       static const QString Connection;
       static const QString Server;
       static const QString ContentLength;
    };


    //Response(int, const QByteArray&);
    QByteArray toByteArray();
    void setStatus(int s);
    void addBody(const QByteArray& b);
    void addBody(const zActionResult& a);
    void addHeaderField(const QString& k, const QString& v);
    //https://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.2.1
    // van válasz

    QString getStatus();

    struct statusCode
    {
       //Informational
        static const int Continue;
        static const int  SwitchingProtocols;

       //       Successful
       // https://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.2.1
       // teljesült, az eredmény leírja, vagy tartalmazza a kért adatot
       static const int OK;
       static const int Created;
       static const int Accepted;
       static const int NonAuthoritativeInformation;
       static const int NoContent;
       static const int ResetContent;
       static const int PartialContent;
       //Redirection
       static const int MultipleChoices;
       static const int MovedPermanently;
       static const int Found;
       static const int SeeOther;
       static const int NotModified;
       static const int UseProxy;
       static const int TemporaryRedirect;
       //Client Error
       static const int BadRequest;
       static const int Unauthorized;
       static const int PaymentRequired;
       static const int Forbidden;
       static const int NotFound;
       static const int MethodNotAllowed;
       static const int NotAcceptable;
       static const int ProxyAuthenticationRequired;
       static const int RequestTimeout;
       static const int Conflict;
       static const int Gone;
       static const int LengthRequired;
       static const int PreconditionFailed;
       static const int RequestEntityTooLarge;
       static const int RequestURItooLarge;
       static const int UnsupportedMediaType;
       static const int RequestedRangeNotSatisfiable;
       static const int ExpectationFailed;
        //Server Error
       static const int InternalServerError;
       static const int NotImplemented;
       static const int BadGateway;
       static const int ServiceUnavailable;
       static const int GatewayTimeout;
       static const int HTTPversionNotSupported;
    };

    static const QString HTTPversion;
    static const QString keyValuePattern;
    static const QString statusLinePattern;

    static QString statusPhrase(int);

private:
    static const QMap<int, QString> statuses;

    int statusCode;
    QByteArray body;
    QMap<QString, QString> headerFields;
};

#endif // RESPONSE_H

