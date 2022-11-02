#include "settings.h"

Settings::Settings(QObject *parent) :
    QSettings(
        QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
        "/org.k_bsp.sail-music/sail-music.conf",
        QSettings::NativeFormat,
        parent
        )
{

}
