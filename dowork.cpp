#include "dowork.h"
#include "zlog.h"
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#ifdef RPI
#include <wiringPi.h>
#endif
//#include <linux/spi/spidev.h>

extern zLog zlog;

doWork::doWork()
{

}

zActionResult doWork::dolgoz1(const QUrlQuery& param, const QByteArray& content)
{
    QJsonObject j
    {
        {"kulcs1", QStringLiteral("érték1")},
        {"kulcs2", QStringLiteral("érték2")},
        {"kulcs3", 55}
    };

#ifdef RPI
    if(param.hasQueryItem("led"))
    {
        int ledState;
        if(param.queryItemValue("led")=="on")
        {
            ledState = HIGH;//digitalWrite (1, HIGH);
        }
        else
        {
            ledState = LOW;//digitalWrite (1, LOW);
        }
        digitalWrite (1, ledState);
        j.insert("LED", ledState);
    }
#endif

    return zActionResult(QJsonDocument(j).toJson(QJsonDocument::Indented), zAction::contentType::JSON);
}

zActionResult doWork::dolgoz2(const QUrlQuery& param, const QByteArray& content)
{                
    return zActionResult(QStringLiteral("árvíztűrőtükörfúrógép 2 :%1").arg(param.query()).toUtf8(), zAction::contentType::TEXT);
}

zActionResult doWork::getNextNumber(const QUrlQuery& param, const QByteArray& content)
{
    param.hasQueryItem("clId");

    static int n = 55;

    QJsonObject j
    {
        {"number", ++n}
    };

    zlog.message(QStringLiteral("new number: %1").arg(n));

    QEventLoop loop;
    QTimer::singleShot(1000*5, &loop, SLOT(quit()));
    loop.exec();

    return zActionResult(QJsonDocument(j).toJson(QJsonDocument::Indented), zAction::contentType::JSON);
}

zActionResult doWork::getHeaderUCtr1(const QUrlQuery& param, const QByteArray& content)
{
    QJsonObject j;

    if(param.hasQueryItem("lc"))
    {
        if(param.queryItemValue("lc")=="hu")
        {
            j.insert("Production", "Gyártási azonosító");
            j.insert("User", "Felh.");
            j.insert("DateTimeFormat", "yyyy.MM.dd.hh:mm:ss");
        }
        else if(param.queryItemValue("lc")=="en")
        {
            j.insert("Production", "Production id");
            j.insert("User", "User");
            j.insert("DateTimeFormat", "MM/dd/yyyy hh:mm:ss");
        }
    }

    return zActionResult(QJsonDocument(j).toJson(QJsonDocument::Indented), zAction::contentType::JSON);
}

zActionResult doWork::getMainPagetr1(const QUrlQuery& param, const QByteArray& content)
{
    QJsonObject j;

    if(param.hasQueryItem("lc"))
    {
        if(param.queryItemValue("lc")=="hu")
        {
            j.insert("Octabin", "Oktabin azon.");
            j.insert("doWork", "Bevitel");

            j.insert("Quality", "Minőség érték");
            j.insert("QualityList", "Bevitel/Megtekintés");

            j.insert("Silo", "Siló azon.");
            j.insert("SiloCorr", "Korrekció");
            j.insert("SiloSelect", "Kiválasztás");

            j.insert("SiloEndState", "Siló vége állapot");
            j.insert("SiloStartState", "Siló start állapot");
        }
        else if(param.queryItemValue("lc")=="en")
        {
            j.insert("Octabin", "Octabin id");
            j.insert("doWork", "Enter");

            j.insert("Quality", "Quality value");
            j.insert("QualityList", "Input/View list");

            j.insert("Silo", "Silo id");
            j.insert("SiloCorr", "Correction");
            j.insert("SiloSelect", "Select");

            j.insert("SiloEndState", "Silo ending qa.");
            j.insert("SiloStartState", "Silo starting qa.");
        }
    }

    return zActionResult(QJsonDocument(j).toJson(QJsonDocument::Indented), zAction::contentType::JSON);
}

//active
zActionResult doWork::testconnection(const QUrlQuery& param, const QByteArray& content)
{
    return zActionResult(QStringLiteral("true").toUtf8(), zAction::contentType::JSON);
}

//active
//LoginResponse() { ErrorMessage = "", Name = "Jasoby Gibbs", UserName = "JG", UserId = 55 };
/*
 * class LoginRequest
    {
        [JsonProperty("username")]
        public string UserName { get; set; }

        [JsonProperty("password")]
        public string Password { get; set; }
    }
*/
zActionResult doWork::login(const QUrlQuery& param, const QByteArray& content)
{
    QJsonObject j;

    j.insert("ErrorMessage", "");
    j.insert("UserId", 55);
    j.insert("UserName", "JG55");
    j.insert("Name", "Jasoby Gibbs");

    return zActionResult(QJsonDocument(j).toJson(QJsonDocument::Indented), zAction::contentType::JSON);
}
