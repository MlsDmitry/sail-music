#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QGuiApplication>

#include "settings.h"
#include "Core.h"
#include "YaClient.h"

#define PACKAGE_NAME "org.k_bsp.sailmusic"

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/sail-music.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).


    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    app->setOrganizationName(QStringLiteral("org.k_bsp"));
    app->setApplicationName(QStringLiteral("SailMusic"));

//    Core core;

//    QQmlContext *context = view.data()->rootContext();
//    context->setContextProperty("core", &core);

    qmlRegisterSingletonType<Core>(PACKAGE_NAME, 1, 0, "Core", &Core::qmlInstance);
    qmlRegisterSingletonType<YaClient>(PACKAGE_NAME, 1, 0, "YaClient", &YaClient::qmlInstance);

    view->setSource(SailfishApp::pathTo("qml/sail-music.qml"));
    view->show();

    return app->exec();
}
