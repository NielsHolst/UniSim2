/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_NODE_H
#define BASE_NODE_H
#include <QString>
#include <QVector>

namespace base {

class Box;

class Node 
{
public:
    enum class Namespace{Include, Exclude};

    struct Type {
        QString namespaceName, className;
        QString toText(Namespace ns = Namespace::Exclude) const;
    };
    using Pedigree = QVector<Type>;

    Node(QString name, Node *parent);
    virtual ~Node();
    void setParent(Node *parent);
    void setClassName(QString nameSpaceName, QString className);
    void setObjectName(QString name);
    void setOrder(int order);
    static void enumerate();

    template <class T=Node*> T parent() const;
    template <class T=Node*> QVector<T> children();
    template <class T=Node*> QVector<T> descendants();

    QString name()       const { return _objectName; }
    QString objectName() const { return _objectName; }
    QString fullName() const;
    static QString fullName(const Node *object);

    QString className(Namespace ns = Namespace::Exclude) const;
    const Pedigree & pedigree() const { return _pedigree; };
    QString pedigreeAsString(Namespace ns = Namespace::Exclude) const;
    bool isType(QString name) const;

    int order() const       { return _order; }

private:
    Pedigree _pedigree;
    QString _objectName;
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

inline bool operator==(const Node::Type &a, const Node::Type &b) {
    return a.namespaceName==b.namespaceName && a.className==b.className;
}

}
#endif
