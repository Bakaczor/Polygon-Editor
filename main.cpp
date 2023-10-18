#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QPainter>
#include <QQuickView>
#include <QPoint>

#include "SceneManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    SceneManager manager;
    manager.painter->drawLine(QPoint(300, 300), QPoint(400, 400));


    QQmlApplicationEngine engine;
    engine.addImageProvider(QString("SceneManager"), &manager);
    engine.rootContext()->setContextProperty("SceneManager", &manager);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Polygon-Editor", "Main");
    return app.exec();
}
