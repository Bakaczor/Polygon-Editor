#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QPainter>
#include <QQuickView>
#include <QPoint>

#include "Painter.h"
#include "Vertex.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Painter>("Painter", 1, 0, "Painter");
    Painter p;
    Vertex v(QPoint(300, 300));



    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Polygon-Editor", "Main");

    return app.exec();
}
