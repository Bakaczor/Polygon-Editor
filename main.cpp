#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QPainter>
#include <QQuickView>
#include "Vertex.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Polygon-Editor", "Main");

    QQuickView view;

    Vertex vertex;
    vertex.setPosition(QPointF(300, 300));


    QObject::connect(&view, &QQuickView::statusChanged, [&vertex, &view](QQuickView::Status status) {
        if (status == QQuickView::Ready) {
            // Create a painter and paint the Vertex
            QPainter painter(view);
            painter.setRenderHint(QPainter::Antialiasing);
            vertex.paint(&painter);
        }
    });

    view.show();

    return app.exec();
}
