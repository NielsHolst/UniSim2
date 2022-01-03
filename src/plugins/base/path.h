/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PATH_H
#define BASE_PATH_H

#include <QObject>
#include <QList>
#include <QVector>

namespace base {

class Box;
class Port;

class Path {
public:
    // Types
    enum class Directive {
        None, Descendants, Ancestors, Siblings, Preceding, Following
    };
    struct Leg {
        Directive directive;
        QString className, objectName;
    };

    // Build
    Path(QObject *parent = nullptr);
    void addLeg(const Leg &leg);

    void hasRoot(bool has) {
        _hasRoot = has;
    }

    // Query
    QString asString() const;

    // Resolve
    const QList<QObject*> & resolved();

private:
    // Data
    QObject *_parent;
    QVector<Leg> _legs;
    bool _hasRoot;

    using Objects = QList<QObject*>;
    Objects _candidates;
    bool _resolved;

    // Methods
    Objects descendants(QObject *p, QString className, QString objectName);
    Objects descendants(Objects candidates, QString className, QString objectName);

    QObject* parent(QObject *p, QString className, QString objectName);
    Objects parents(Objects candidates, QString className, QString objectName);

    Objects ancestors(QObject *p, QString className, QString objectName);
    Objects ancestors(Objects candidates, QString className, QString objectName);

    Objects children(QObject *p, QString className, QString objectName);
    Objects children(Objects candidates, QString className, QString objectName);

    Objects siblings(QObject *p, QString className, QString objectName);
    Objects siblings(Objects candidates, QString className, QString objectName);

    QObject* preceding(QObject *p, QString className, QString objectName);
    Objects preceding(Objects candidates, QString className, QString objectName);

    QObject* following(QObject *p, QString className, QString objectName);
    Objects following(Objects candidates, QString className, QString objectName);

    void filter(QVector<Leg>::const_iterator next);

};


}

#endif
