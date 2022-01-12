/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "exception.h"
#include "node.h"
#include "port.h"

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

void Node::enumerate() {
    Box *root = Box::root();
    if (!root)
      ThrowException("Missing root");

    int i = 0;
    Box::root()->enumerate(i);

    root->orderPorts();
    auto all = root->descendants<Box*>() << root;
    for (auto *box : all) {
        QString s = fullName(box);
        box->orderPorts();
    }
}

void Node::enumerate(int &i) {
    auto children_ = children<Node*>();
    for (auto *node : children_) {
        QString s = fullName(node);
        node->enumerate(i);
    }
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
    const Node *p = object;
    const Node *last;
    while (p) {
        names.prepend(p->objectName());
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
    auto parentNode = parent();
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
