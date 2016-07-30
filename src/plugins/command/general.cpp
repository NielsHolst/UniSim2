#include <base/port.h>
#include "general.h"

using namespace base;

namespace command {

namespace {
    QString typeInfo(QObject *object) {
        QString s = className(object);
        Port *port = dynamic_cast<Port*>(object);
        if (port)
            s += " " + nameOf(port->type());
        return s;
    }

    int maxWidth(QVector<QObject*> objects) {
        int width(0);
        for (QObject *object : objects) {
            int w = typeInfo(object).size();
            if (w > width) width = w;
        }
        return width;
    }
}

QString info(QVector<QObject*> objects) {
    QStringList list;
    int W = maxWidth(objects);
    for (QObject *object : objects) {
        int width = typeInfo(object).size();
        QString filler = QString().fill(' ', W - width + 1);
        QString s = typeInfo(object) + filler + fullName(object);
        list << s;
    }
    return list.join("\n");
}

QString info(QObject *object) {
    QVector<QObject*> objects;
    objects << object;
    return info(objects);
}

}
