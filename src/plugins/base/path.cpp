/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMap>
#include <QVector>
#include "box.h"
#include "class_name.h"
#include "environment.h"
#include "exception.h"
#include "path.h"

using namespace std;

namespace base {

static QMap<QString, Path::Directive> directives =
{
    {"None",        Path::Directive::None},
    {"Descendants", Path::Directive::Descendants},
    {"Ancestors",   Path::Directive::Ancestors},
    {"Siblings",    Path::Directive::Siblings},
    {"Preceding",   Path::Directive::Preceding},
    {"Following",   Path::Directive::Following}
};

static QMap<Path::Directive, QString> directivesStr =
{
    {Path::Directive::None,        "None"       },
    {Path::Directive::Descendants, "Descendants"},
    {Path::Directive::Ancestors  , "Ancestors"  },
    {Path::Directive::Siblings   , "Siblings"   },
    {Path::Directive::Preceding  , "Preceding"  },
    {Path::Directive::Following  , "Following"  }
};

Path::Path(QObject *parent)
    : _parent(parent), _resolved(false)
{
}

void Path::addLeg(const Leg &leg) {
    _legs << leg;
    if (leg.objectName.contains(".") && leg.directive != Directive::None)
        ThrowException ("Path with dot notation cannot be combined with a directive").value(asString());
}


QString Path::asString() const {
    QStringList sl;
    for (auto leg : _legs) {
        QString s = leg.objectName;
        if (leg.className != "Box")
            s.prepend(leg.className + "::");
        if (leg.directive != Directive::None)
            s.prepend(directivesStr.value(leg.directive) + "::");
    }
    QString s = sl.join("/");
    if (_hasRoot)
        s.prepend("/");
    return s;
}

const QList<QObject*> & Path::resolved() {
    if (!(_parent || _hasRoot))
        ThrowException("Path must have a root or a parent").value(asString());

    if (_resolved)
        return _candidates;

    _candidates.clear();
    if (_legs.isEmpty())
        return _candidates;

    auto next = _legs.begin();
    const Leg &leg(*next);

    if (_hasRoot) {
        _candidates << environment().root();
    }
    else if (leg.objectName == ".") {
        _candidates << _parent;
        ++next;
    }
    else if (leg.objectName == "..") {
        _candidates << parent(_parent, "", "*");
        ++next;
    }
    else if (leg.objectName == "...") {
        _candidates << _parent << ancestors(_parent, "", "*");
        ++next;
    }
    else {
        _candidates << _parent;
    }
    for(;next != _legs.end(); ++next)
        filter(next);

    _resolved = true;
    return _candidates;
}

void Path::filter(QVector<Leg>::const_iterator next) {
    const Leg &leg(*next);
    if (leg.objectName == ".") {
        ; // keep candidates
    }
    else if (leg.objectName == "..") {
        _candidates = parents(_candidates, "", "*");
    }
    else if (leg.objectName == "...") {
        _candidates << ancestors(_candidates, "", "*");
    }
    else {
        switch(leg.directive) {
        case Directive::None:
        case Directive::Descendants:
            _candidates = descendants(_candidates, leg.className, leg.objectName);
            break;
        case Directive::Ancestors:
            _candidates = ancestors(_candidates, leg.className, leg.objectName);
            break;
        case Directive::Siblings:
            _candidates = siblings(_candidates, leg.className, leg.objectName);
            break;
        case Directive::Preceding:
            _candidates = preceding(_candidates, leg.className, leg.objectName);
            break;
        case Directive::Following:
            _candidates = following(_candidates, leg.className, leg.objectName);
            break;
        }
    }
}


inline bool match(QObject *p, QString className, QString objectName) {
    return (className.isEmpty() || hasClassName(p, className)) &&
           (objectName == "*"   || p->objectName() == objectName);
}

Path::Objects Path::descendants(QObject *p, QString className, QString objectName) {
    Objects result;
    auto descendants = p->findChildren<QObject*>();
    for (auto descendant : descendants) {
        if (match(descendant, className, objectName))
            result << descendant;
    }
    return result;
}

Path::Objects Path::descendants(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates)
        result << descendants(candidate, className, objectName);
    return result;
}

QObject* Path::parent(QObject *p, QString className, QString objectName) {
    auto parent = p->parent();
    return (parent && match(parent, className, objectName)) ? parent : nullptr;
}

Path::Objects Path::parents(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates) {
        auto p = parent(candidate, className, objectName);
        if (p)
            result << p;
    }
    return result;
}

Path::Objects Path::ancestors(QObject *p, QString className, QString objectName) {
    Objects result;
    auto q = p->parent();
    if (q) {
        if (match(q, className, objectName))
            result << q;
        result << ancestors(q, className, objectName);
    }
    return result;
}

Path::Objects Path::ancestors(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates)
        result << ancestors(candidate, className, objectName);
    return result;
}

Path::Objects Path::children(QObject *p, QString className, QString objectName) {
    Objects result;
    auto children = p->findChildren<QObject*>(QString(), Qt::FindDirectChildrenOnly);
    for (auto child : children) {
        if (match(child, className, objectName))
            result << child;
    }
    return result;
}

Path::Objects Path::children(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates)
        result << children(candidate, className, objectName);
    return result;
}

Path::Objects Path::siblings(QObject *p, QString className, QString objectName) {
    Objects result;
    auto children = p->findChildren<QObject*>(QString(), Qt::FindDirectChildrenOnly);
    for (auto child : children) {
        if (child != p && match(child, className, objectName))
            result << child;
    }
    return result;
}

Path::Objects Path::siblings(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates)
        result << siblings(candidate, className, objectName);
    return result;
}

QObject* Path::preceding(QObject *p, QString className, QString objectName) {
    auto children = p->findChildren<QObject*>(QString(), Qt::FindDirectChildrenOnly);
    int i=0;
    while (children.at(i) != p) ++i;
    return (i>0 && match(children.at(i), className, objectName)) ? children[i] : nullptr;
}

Path::Objects Path::preceding(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates) {
        auto p = preceding(candidate, className, objectName);
        if (p)
            result << p;
    }
    return result;
}

QObject* Path::following(QObject *p, QString className, QString objectName) {
    auto children = p->findChildren<QObject*>(QString(), Qt::FindDirectChildrenOnly);
    int i=0,
        n=children.size();
    while (children.at(i) != p) ++i;
    return (i<n-1 && match(children.at(i+1), className, objectName)) ? children[i+1] : nullptr;
}

Path::Objects Path::following(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates) {
        auto p = following(candidate, className, objectName);
        if (p)
            result << p;
    }
    return result;
}

} //namespace
