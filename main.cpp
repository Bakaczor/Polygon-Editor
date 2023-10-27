#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>

#include "SceneManager.h"
#include "ImageProvider.h"
#include "Enums.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QPointer<SceneManager> manager = new SceneManager();
    QPointer<ImageProvider> provider = new ImageProvider(manager->painter, manager->image);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("SceneManager", manager);
    engine.addImageProvider(QString("ImageProvider"), provider);

    qmlRegisterUncreatableType<Algorithm>("com.algorithm.enum", 1, 0, "Algorithm", "Cannot create Algorithm in QML");
    qmlRegisterUncreatableType<Orientation>("com.orientation.enum", 1, 0, "Orientation", "Cannot create Orientation in QML");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Polygon-Editor", "Main");

    manager->addVertex(100, 100);
    manager->addVertex(200, 100);
    manager->addVertex(200, 200);
    manager->addVertex(100, 200);
    manager->addVertex(100, 100);

    return app.exec();
}
