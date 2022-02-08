/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "exception.h"
#include "node.h"
#include "port.h"

#include <iostream>

namespace base {

Node::Node(QString name, Node *parent)
    : _name(name), _parent(parent), _order(-1)
{
    if (parent)
        parent->addChild(this);
}

Node::~Node() {
    for (auto child : _children)
        delete child;
    _children.clear();
}

void Node::addChild(Node *child) {
    _children << child;
}

void Node::setParent(Node *parent) {
    if (_parent)
        ThrowException("Parent already set").value(parent->fullName()).context(this);
    _parent = parent;
    if (parent)
        parent->addChild(this);
}

void Node::setClassName(QString name) {
    auto list = name.split("::");
    if (list.size() > 2)
        ThrowException("Invalid class name").value(name).context(this);
    _class = list.last();
}

void Node::setObjectName(QString name) {
    _name = name;
}

void Node::setOrder(int order) {
    _order = order;
}

void Node::enumerate() {
    Box *root = Box::root();
    if (!root)
      ThrowException("Missing root");
    int i = 0;
    root->enumerate(i);
}

void Node::enumerate(int &i) {
    for (auto *box : children<Node*>())
        box->enumerate(i);
    _order = i++;
}

QString Node::objectName() const {
    return _name;
}

QString Node::fullName() const {
    return fullName(this);
}

QString Node::fullName(const Node *object) {
    QStringList names;
    QString portName("");
    const Node *p = object;
    if (dynamic_cast<const Port*>(p)) {
        portName = "[" + p->objectName() + "]";
        p = p->parent();
    }
    while (p) {
        names.prepend(p->objectName());
        p = p->parent();
    }
    return names.join("/") + portName;
}

QString Node::className(Namespace ns) const {
    return (ns == Namespace::Include) ? (namespaceName() + "::" + _class) : _class;
}

//QStringList Node::pedigree(Namespace ns) const {
//    QStringList result;
//    auto parentNode = parent();
//    if (parentNode)
//        result = parentNode->pedigree(ns) + result;
//    else
//        result << className(ns);
//    return result;
//}

bool Node::isType(QString name) const {
    bool ok = name==className(Namespace::Include) || name==className(Namespace::Exclude);
    return ok;

//    Namespace ns = name.contains("::") ? Namespace::Include : Namespace::Exclude;
//    QStringList test = pedigree(ns);
//    return pedigree(ns).contains(name);
}

template <> QVector<Box*> Node::children() {
    QVector<Box*> result;
    for (auto child : _children) {
        Box* res = dynamic_cast<Box*>(child);
        if (res) result << res;
    }
    return result;
}

}
