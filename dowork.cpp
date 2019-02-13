#include "dowork.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <wiringPi.h>
//#include <linux/spi/spidev.h>

doWork::doWork()
{

}

zActionResult doWork::dolgoz1(const QUrlQuery& param)
{
    QJsonObject j
    {
        {"kulcs1", QStringLiteral("érték1")},
        {"kulcs2", QStringLiteral("érték2")},
        {"kulcs3", 55},
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

zActionResult doWork::dolgoz2(const QUrlQuery& param)
{                
    return zActionResult(QStringLiteral("árvíztűrőtükörfúrógép 2 :%1").arg(param.query()).toUtf8(), zAction::contentType::TEXT);
}
