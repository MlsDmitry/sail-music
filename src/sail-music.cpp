#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>
#include <sailfishapp.h>

#include <ctime>
#include <json_dto/pub.hpp>
#include <string>

#include "Utils/Core.h"
#include "Utils/Settings.h"

#include "Models/ArtistModel.h"
#include "Models/TrackModel.h"
#include "Models/YaClientModel.h"

#include "Radio/RadioListModel.h"

#define PACKAGE_NAME "org.k_bsp.sailmusic"

// Message.
struct message_t
{
    message_t()
    {
    }

    message_t(std::string from, std::int64_t when, std::string text)
        : m_from{std::move(from)}, m_when{when}, m_text{std::move(text)}
    {
    }

    // Who sent a message.
    std::string m_from;
    // When the message was sent (unixtime).
    std::int64_t m_when;
    // Message text.
    std::string m_text;

    template <typename Json_Io> void json_io(Json_Io &io)
    {
        io &json_dto::mandatory("from", m_from) & json_dto::mandatory("when", m_when) &
            json_dto::mandatory("text", m_text);
    }
};

const std::string json_data{
    R"JSON({
  "from" : "json_dto",
  "when" : 1474884330,
  "text" : "Hello world!"
})JSON"};

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

    try
    {
        {
            auto msg = json_dto::from_json<message_t>(json_data);

            const auto t = static_cast<std::time_t>(msg.m_when);
            std::cout << "Deserialized from JSON:\n"
                      << "\tfrom: " << msg.m_from << "\n"
                      << "\twhen: " << std::ctime(&t) << "\ttext: " << msg.m_text << std::endl;
        }

        {
            const message_t msg{"json_dto", std::time(nullptr), "Hello once again!"};

            std::cout << "\nSerialized to JSON:\n" << json_dto::to_json(msg) << std::endl;
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << "." << std::endl;
        return 1;
    }

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    app->setOrganizationName(QStringLiteral("org.k_bsp"));
    app->setApplicationName(QStringLiteral("SailMusic"));

    qRegisterMetaType<Track *>();
    qRegisterMetaType<Artist *>();

    qmlRegisterSingletonType<Core>(PACKAGE_NAME, 1, 0, "Core", &Core::qmlInstance);
    qmlRegisterSingletonType<YaClient>(PACKAGE_NAME, 1, 0, "YaClient", &YaClient::qmlInstance);

    qmlRegisterType<RadioListModel>(PACKAGE_NAME, 1, 0, "RadioListModel");

    view->setSource(SailfishApp::pathTo("qml/sail-music.qml"));
    view->show();

    return app->exec();
}
