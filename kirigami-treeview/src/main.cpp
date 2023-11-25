// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QUrl>
#ifdef Q_OS_ANDROID
#include <GuiQApplication>
#else
#include <QApplication>
#endif

#include "app.h"
#include "version-ktreeviewtest.h"
#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "ktreeviewtestconfig.h"
#include "itemmodeltest.h"

#ifdef Q_OS_ANDROID
Q_DECL_EXPORT
#endif
int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#ifdef Q_OS_ANDROID
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle(QStringLiteral("org.kde.breeze"));
#else
    QApplication app(argc, argv);

    // Default to org.kde.desktop style unless the user forces another style
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }
#endif

#ifdef Q_OS_WINDOWS
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }

    QApplication::setStyle(QStringLiteral("breeze"));
    auto font = app.font();
    font.setPointSize(10);
    app.setFont(font);
#endif

    KLocalizedString::setApplicationDomain("ktreeviewtest");
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));

    KAboutData aboutData(
        // The program name used internally.
        QStringLiteral("ktreeviewtest"),
        // A displayable program name string.
        i18nc("@title", "KTreeViewTest"),
        // The program version string.
        QStringLiteral(KTREEVIEWTEST_VERSION_STRING),
        // Short description of what the app does.
        i18n("Application Description"),
        // The license this code is released under.
        KAboutLicense::GPL,
        // Copyright Statement.
        i18n("(c) %{CURRENT_YEAR}"));
    aboutData.addAuthor(i18nc("@info:credit", "%{AUTHOR}"),
                        i18nc("@info:credit", "Maintainer"),
                        QStringLiteral("%{EMAIL}"),
                        QStringLiteral("https://yourwebsite.com"));
    aboutData.setTranslator(i18nc("NAME OF TRANSLATORS", "Your names"), i18nc("EMAIL OF TRANSLATORS", "Your emails"));
    KAboutData::setApplicationData(aboutData);
    QGuiApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("org.kde.ktreeviewtest")));

    QQmlApplicationEngine engine;

    auto config = KTreeViewTestConfig::self();

    qmlRegisterSingletonInstance("org.kde.ktreeviewtest", 1, 0, "Config", config);

    qmlRegisterSingletonType("org.kde.ktreeviewtest", 1, 0, "About", [](QQmlEngine *engine, QJSEngine *) -> QJSValue {
        return engine->toScriptValue(KAboutData::applicationData());
    });

    App application;
    qmlRegisterSingletonInstance("org.kde.ktreeviewtest", 1, 0, "App", &application);

    // BEGIN FeedFolder test
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
    // END FeedFolder test

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
