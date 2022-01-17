/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_NODE_H
#define BASE_NODE_H
#include <QString>
#include <QVector>

#define APOSTROPHED(x) #x

namespace base {

class Box;

class Node 
{
public:
    enum class Namespace {Include, Exclude};

    Node(QString name, Node *parent);
    virtual ~Node();
    void setParent(Node *parent);
    void setClassName(QString name);
    void setObjectName(QString name);
    void setOrder(int order);
    static void enumerate();

    template <class T=Node*> T parent() const;
    template <class T=Node*> QVector<T> children();
    template <class T=Node*> QVector<T> descendants();

    QString objectName() const;
    QString className(Namespace ns = Namespace::Exclude) const;

    QString fullName() const;
    static QString fullName(const Node *object);

    QStringList pedigree(Namespace ns = Namespace::Exclude) const;
    bool isType(QString name) const;

    QString name() const    { return objectName(); }
    int order() const       { return _order; }

    // Namespace is defined by BOXES_PLUGIN_NAME in project file
    QString namespaceName() const { return APOSTROPHED(BOXES_PLUGIN_NAME); }

private:
    QString _name, _class;
    Node *_parent;
    QVector<Node*> _children;
    int _order;
    void addChild(Node *child);
    void enumerate(int &i);
    template <class T=Node*> QVector<T> descendants(bool includeMe);
};

template <class T> T Node::parent() const {
    return dynamic_cast<T>(_parent);
}

template <class T> QVector<T> Node::children() {
    QVector<T> result;
    for (auto child : _children) {
        T res = dynamic_cast<T>(child);
        if (res) result << res;
    }
    return result;
}

template <> inline QVector<Node*> Node::children() {
    return _children;
}

template <> QVector<Box*> Node::children();

template <class T> QVector<T> Node::descendants() {
    return descendants<T>(false);
}

template <class T> QVector<T> Node::descendants(bool includeMe) {
    QVector<T> result;
    for (auto child : _children)
        result.append(child->descendants<T>(true));
    if (includeMe) {
        T res = dynamic_cast<T>(this);
        if (res) result << res;
    }
    return result;
}

inline bool operator<(const Node &a, const Node &b) {
    return a.order() < b.order();
}

}
#endif
