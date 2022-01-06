/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "exception.h"
#include "node.h"
#include "port.h"

namespace base {

Node::Node(QString name, QObject *parent)
    : QObject(parent), _order(-1)
{
    setObjectName(name);
}

void Node::setClassName(QString className) {
    auto list = className.split("::");
    if (list.size() > 2)
        ThrowException("Invalid class name").value(className).context(this);
    _class = list.last();
}

void Node::enumerate(int &i) {
    // Enumerate boxes before ports
    for (auto box : findChildren<Box*>("",Qt::FindDirectChildrenOnly))
        box->enumerate(i);
    for (auto port : findChildren<Port*>("",Qt::FindDirectChildrenOnly))
        port->enumerate(i);
    // Enumerate self
    _order = i++;
}

QString Node::fullName() const {
    return fullName(this);
}

QString Node::fullName(const QObject *object) {
    QStringList names;
    const QObject *p = object;
    const QObject *last;
    while (p) {
        names << p->objectName();
        last = p;
        p = p->parent();
    }
    if (dynamic_cast<const Port*>(last)) {
        QString portName = names.last();
        names.pop_back();
        return names.join("/") + "[" + portName + "]";
    }
    else
        return names.join("/");
}

QString Node::className(Namespace ns) const {
    return (ns == Namespace::Include) ? (namespaceName() + "::" + _class) : _class;
}

QStringList Node::pedigree(Namespace ns) const {
    QStringList result;
    auto parentNode = dynamic_cast<Node*>(parent());
    if (parentNode)
        result = parentNode->pedigree() + result;
    else
        result << className(ns);
    return result;
}

bool Node::isType(QString name) const {
    Namespace ns = name.contains("::") ? Namespace::Include : Namespace::Exclude;
    return pedigree(ns).contains(name);
}

}
