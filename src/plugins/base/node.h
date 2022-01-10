/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_NODE_H
#define BASE_NODE_H
#include <QObject>

#define APOSTROPHED(x) #x

namespace base {

class Node : public QObject
{
public:
    enum class Namespace {Include, Exclude};

    Node(QString name, QObject *parent);
    void setClassName(QString className);
    void enumerate(int &i);

    QString fullName() const;
    static QString fullName(const QObject *object);

    QString className(Namespace ns = Namespace::Exclude) const;
    QStringList pedigree(Namespace ns = Namespace::Exclude) const;
    bool isType(QString name) const;

    QString name() const    { return objectName(); }
    int order() const       { return _order; }

    // Namespace is defined by BOXES_PLUGIN_NAME in project file
    QString namespaceName() const { return APOSTROPHED(BOXES_PLUGIN_NAME); }


private:
    QString _class;
    int _order;
};

inline bool operator<(const Node &a, const Node &b) {
    return a.order() < b.order();
}

}
#endif
