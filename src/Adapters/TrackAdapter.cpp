#include "TrackAdapter.h"
#include "Models/TrackModel.h"

#include <QDomDocument>
#include <QJsonArray>
#include <QXmlStreamReader>

Track *parseTrack(QJsonValue &data)
{
    try
    {
        QString trackStr = QJsonDocument(data.toObject()).toJson(QJsonDocument::Compact);
        auto track = json_dto::from_json<track_t>(trackStr.toStdString());

        Track *trackModel = new Track();

        trackModel->id = QString::fromStdString(track._id);
        trackModel->title = QString::fromStdString(track._title);
        trackModel->coverUrl = "https://" + QString::fromStdString(track._coverUrl);
        trackModel->duration = track._durationMs;

        return trackModel;
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Error: " << ex.what() << ".";
        return nullptr;
    }
}

QVariantMap parseDownloadInfo(QJsonValue &data)
{
    QVariantMap downloadInfos;
    try
    {
        QJsonArray infos = data.toArray();
        for (QJsonValue infoJson : infos)
        {
            QString infoStr = QJsonDocument(infoJson.toObject()).toJson(QJsonDocument::Compact);
            auto info = json_dto::from_json<track_download_info_t>(infoStr.toStdString());

            if (info._codec == "mp3")
                downloadInfos.insert(QString::number(info._bitrate), QString::fromStdString(info._downloadInfoUrl));
        }
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Error: " << ex.what() << ".";
        return QVariantMap();
    }

    return downloadInfos;
}

track_file_download_info parseTrackFileDownloadInfo(QByteArray data)
{
    track_file_download_info info;
    QXmlStreamReader reader(data);

    if (reader.readNextStartElement())
    {

        if (reader.name() == "download-info")
        {
            while (reader.readNextStartElement())
            {
                QStringRef elName = reader.name();

                if (elName == "host")
                {
                    info.host = reader.readElementText();
                }
                else if (elName == "s")
                {
                    info.signature = reader.readElementText();
                }
                else if (elName == "ts")
                {
                    info.ts = reader.readElementText();
                }
                else if (elName == "path")
                {
                    info.path = reader.readElementText();
                }
                else
                {
                    reader.skipCurrentElement();
                }
            }
        }
    }

    return info;
}
