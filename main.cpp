#include "qqml.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "boardcanvas.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<BoardCanvas>("BoardCanvas",1,0,"BoardCanvas");
    const QUrl url(u"qrc:/Vast/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
