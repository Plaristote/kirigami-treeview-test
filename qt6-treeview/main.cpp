#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "itemmodeltest.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/QItemModelTest/Main.qml"_qs);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    FeedFolder rootFolder;

    FeedFolder* subFolder = new FeedFolder();
    subFolder->setProperty("name", QStringLiteral("Folder"));

    Feed* feed1 = new Feed();
    Feed* feed2 = new Feed();
    Feed* feed3 = new Feed();

    feed1->setProperty("name", QStringLiteral("Feed1"));
    feed2->setProperty("name", QStringLiteral("Feed2"));
    feed3->setProperty("name", QStringLiteral("Feed3"));

    subFolder->addMenuItem(feed1);
    subFolder->addMenuItem(feed2);
    rootFolder.addMenuItem(subFolder);
    rootFolder.addMenuItem(feed3);

    engine.rootContext()->setContextProperty(QStringLiteral("rootFolder"), &rootFolder);
    engine.load(url);

    return app.exec();
}
