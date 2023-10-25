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

    QQmlApplicationEngine engine;
    engine.addImageProvider(QString("SceneManager"), &manager);
    engine.rootContext()->setContextProperty("SceneManager", &manager);


    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Polygon-Editor", "Main");

    manager.addVertex(100, 100);
    manager.addVertex(200, 100);
    manager.addVertex(200, 200);
    manager.addVertex(100, 200);
    manager.addVertex(100, 100);

    return app.exec();
}
