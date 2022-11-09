#include "AlbumAdapter.h"

#include <QDebug>

Album*
parseAlbum(QJsonValue& data)
{
    try
    {
        QString albumStr = QJsonDocument(data.toObject()).toJson(QJsonDocument::Compact);
        auto album = json_dto::from_json<album_t>(albumStr.toStdString());

        Album* albumModel = new Album();

        albumModel->id = QString::number(album._id);
        albumModel->title = QString::fromStdString(album._title);
        albumModel->coverUrl = "https://" + QString::fromStdString(album._coverUrl);

        return albumModel;

    } catch ( const std::exception & ex )
    {
        qDebug() << "Error: " << ex.what() << ".";
        return nullptr;
    }
}
