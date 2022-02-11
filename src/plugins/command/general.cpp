/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/node.h>
#include <base/port.h>
#include "general.h"

using namespace base;

namespace command {

namespace {
    QString typeInfo(Node *node) {
        QString s = node->className();
        Port *port = dynamic_cast<Port*>(node);
        if (port)
            s += " " + port->value().typeName();
        return s;
    }

    int maxWidth(QVector<Node*> nodes) {
        int width(0);
        for (auto node : nodes) {
            int w = typeInfo(node).size();
            if (w > width) width = w;
        }
        return width;
    }
}

QString info(QVector<Node*> nodes) {
    QStringList list;
    int W = maxWidth(nodes);
    for (auto node : nodes) {
        int width = typeInfo(node).size();
        QString filler = QString().fill(' ', W - width + 1);
        QString s = typeInfo(node) + filler + node->fullName();
        list << s;
    }
    return list.join("\n");
}

QString info(Node *node) {
    return info(QVector<Node*>() << node);
}

}
