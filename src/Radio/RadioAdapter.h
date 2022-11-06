#ifndef RADIOADAPTER_H
#define RADIOADAPTER_H

#include "Adapters/TrackAdapter.h"

#include "json_dto/pub.hpp"

#include <QObject>
#include <QJsonValue>
#include <QVariantList>


QVariantList
parseTracks(QJsonValue& data);

#endif // RADIOADAPTER_H
