/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_STRING_MAP_H
#define UNISIM_STRING_MAP_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <base/exception.h>

#define StringMapEntry(map, tag) map [ #tag ] = tag

namespace vg {

class NamedObject;

template <class T>
class StringMap : public QMap<QString,T>
{
public:
    StringMap();
    QStringList validKeys() const;
    T seek(QString key, const QObject *context=0);
};

template <class T>
StringMap<T>::StringMap()
    : QMap<QString,T>()
{
}

template <class T>
QStringList StringMap<T>::validKeys() const {
    return QStringList(QMap<QString, T>::keys());
}

template <class T>
T StringMap<T>::seek(QString key, const QObject *context) {
    if (!QMap<QString, T>::contains(key)) {
        QString msg = "Unknown key in list. Only these keys are valid: '%1'";
        ThrowException(msg.arg(validKeys().join(","))).value(key).context(context);
    }
    return QMap<QString, T>::value(key);
}

} //namespace

#endif

