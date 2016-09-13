#ifndef BASE_VERSION_H
#define BASE_VERSION_H
#include <QFile>
#include <QStringList>
#include "exception.h"
#include "version.h"

namespace  {
    const QString unknown{"(Unknown version)"};
}

namespace base {

QString versionRaw() {
    QFile file (":/version.txt");
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        return unknown;
    }
    return file.readAll();
}

QString version(QString raw) {
    QStringList items = raw.split("\t");
    if (items.size() != 4) {
        return unknown;
    }
    QString config = items.at(3);
    items.removeAt(3);
    QString code = items.join(".");
    if (config != "release")
        code += " (developer)";
    return code;
}

QString version() {
    return version(versionRaw());
}

}

#endif

