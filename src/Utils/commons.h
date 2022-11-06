#ifndef COMMONS_H
#define COMMONS_H

#include <QString>

qint64 hash(const QString & str);

QString makeUrl(const char * baseUrl, const char *format, ...);

#endif // COMMONS_H
