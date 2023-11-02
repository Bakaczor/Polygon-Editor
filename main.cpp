#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>

#include "SceneManager.h"
#include "ImageProvider.h"

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

    // build K
    manager->addVertex(75, 50);
    manager->addVertex(150, 50);
    manager->addVertex(150, 200);
    manager->addVertex(250, 50);
    manager->addVertex(350, 50);
    manager->addVertex(200, 250);
    manager->addVertex(350, 450);
    manager->addVertex(250, 450);
    manager->addVertex(150, 300);
    manager->addVertex(150, 450);
    manager->addVertex(75, 450);
    manager->addVertex(75, 50);

    manager->currPolIdx = 0;
    manager->currObject = Geometry::Edge;

    manager->currEdgIdx = 0;
    manager->changeOrientation(Orientation::Enum::Horizontal);
    manager->currEdgIdx = 1;
    manager->changeOrientation(Orientation::Enum::Vertical);
    manager->currEdgIdx = 3;
    manager->changeOrientation(Orientation::Enum::Horizontal);
    manager->currEdgIdx = 6;
    manager->changeOrientation(Orientation::Enum::Horizontal);
    manager->currEdgIdx = 8;
    manager->changeOrientation(Orientation::Enum::Vertical);
    manager->currEdgIdx = 9;
    manager->changeOrientation(Orientation::Enum::Horizontal);
    manager->currEdgIdx = 10;
    manager->changeOrientation(Orientation::Enum::Vertical);

    // build benchmark
    manager->addVertex(400, 250);
    manager->addVertex(750, 250);
    manager->addVertex(750, 600);
    manager->addVertex(590, 600);
    manager->addVertex(590, 425);
    manager->addVertex(650, 425);
    manager->addVertex(650, 350);
    manager->addVertex(500, 350);
    manager->addVertex(500, 475);
    manager->addVertex(560, 475);
    manager->addVertex(560, 550);
    manager->addVertex(400, 550);
    manager->addVertex(400, 250);

    manager->currPolIdx = 1;
    manager->currObject = Geometry::Edge;

    manager->currEdgIdx = 0;
    manager->changeOrientation(Orientation::Enum::Horizontal);
    manager->currEdgIdx = 1;
    manager->changeOrientation(Orientation::Enum::Vertical);
    manager->currEdgIdx = 2;
    manager->changeOrientation(Orientation::Enum::Horizontal);
    manager->currEdgIdx = 3;
    manager->changeOrientation(Orientation::Enum::Vertical);
    manager->currEdgIdx = 4;
    manager->changeOrientation(Orientation::Enum::Horizontal);
    manager->currEdgIdx = 5;
    manager->changeOrientation(Orientation::Enum::Vertical);
    manager->currEdgIdx = 6;
    manager->changeOrientation(Orientation::Enum::Horizontal);
    manager->currEdgIdx = 7;
    manager->changeOrientation(Orientation::Enum::Vertical);
    manager->currEdgIdx = 8;
    manager->changeOrientation(Orientation::Enum::Horizontal);
    manager->currEdgIdx = 9;
    manager->changeOrientation(Orientation::Enum::Vertical);
    manager->currEdgIdx = 10;
    manager->changeOrientation(Orientation::Enum::Horizontal);
    manager->currEdgIdx = 11;
    manager->changeOrientation(Orientation::Enum::Vertical);

    manager->currEdgIdx = -1;
    manager->currPolIdx = -1;
    manager->currObject = Geometry::None;

    return app.exec();
}
