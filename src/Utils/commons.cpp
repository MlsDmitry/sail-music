#include <QByteArray>
#include <QCryptographicHash>
#include <QDataStream>
#include <QString>


#include "commons.h"


qint64 hash(const QString & str)
{
    QByteArray hash = QCryptographicHash::hash(
                QByteArray::fromRawData((const char*)str.utf16(), str.length()*2),
                QCryptographicHash::Md5
                );
    Q_ASSERT(hash.size() == 16);
    QDataStream stream(hash);
    qint64 a, b;
    stream >> a >> b;
    return a ^ b;
}

QString
makeUrl(const char * baseUrl, const char *format, ...)
{
    QString url(baseUrl);

    va_list args;
    va_start(args, format);

    QString url2 = QString().vsprintf(format, args);

    url.append(url2);

    va_end(args);

    return url;
}
