/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QRegExp>
#include "box.h"
#include "exception.h"
#include "general.h"
#include "path.h"

using namespace std;

namespace base {

QMap<QString, Path::Directive> Path::_directives;

#define DIR(x) _directives[QString(#x).toLower()] = x

Path::Path(QString path, QObject *context)
    : _originalContext(context), _caller(0)
{
    initDirectives();
    _originalPaths << path;
}

Path::Path(QStringList paths, QObject *context)
    : _originalContext(context), _caller(0)
{
    initDirectives();
    _originalPaths = paths;
}

void Path::initDirectives() {
    // Construct singular directives map
    if (_directives.isEmpty()) {
        DIR(Self);
        DIR(Children);
        DIR(Parent);
        DIR(Nearest);
        DIR(SelfOrDescendants);
        DIR(Descendants);
        DIR(Ancestors);
        DIR(AllSiblings);
        DIR(OtherSiblings);
        DIR(PreceedingSibling);
        DIR(FollowingSibling);
    }
}

QObjectList Path::_resolve(int number, QObject *caller) {
    _caller = caller;
    _candidates.clear();
    for (int i = 0; i <_originalPaths.size(); ++i) {
        normalise(i);
        if (_current.normalisedContext == 0)
            throw Exception("Path misses a context object", _current.originalPath, _caller);
        QObjectList nextCandidates = (QObjectList() << _current.normalisedContext);
        addCandidates(_current.normalisedPath, nextCandidates);
        _candidates << nextCandidates;
    }
    removeEmptyCandidates();
    if (number > -1 && _candidates.size() != number) {
        QString msg{"Path resolves to the wrong number of mathes: found(%1), expected(%2)"};
        throw Exception(msg.arg(_candidates.size()).arg(number), _current.originalPath, _caller);
    }
    return _candidates;
}

QString Path::normalise(int ix) {
    QString result;
     _current.originalPath = _originalPaths.at(ix);
    int leftBracket = _current.originalPath.indexOf('[');
    QStringList base = _current.originalPath.left(leftBracket).split('/');

    if (isAbsolute()) {
        base.removeAt(0);
        if (base.at(0).isEmpty()) base[0] = "*";
    }

    result = normaliseFirstBox(base.at(0));
    for (int i = 1; i < base.size(); ++i)
        result += "/" + normaliseBox(base.at(i));

    QString port = normalisePort();
    if (!port.isEmpty())
        result += "/" + port;

    return _current.normalisedPath = result;
}

void Path::validateName(QString name) {
    validate(QRegExp("[_A-Za-z0-9]+|\\*"), name);
}

void Path::validateStep(QString step) {
    validate(QRegExp("([_A-Za-z0-9]+:?)|\\*|\\.|\\.\\.|\\.\\.\\."), step);
}

void Path::validate(QRegExp rx, QString s) {
    QString value = "'%1'' in '%2'";
    if (!rx.exactMatch(s))
        throw Exception("Bad path format", value.arg(s).arg(_current.originalPath), _originalContext);
}

QString Path::normaliseFirstBox(QString s) {
    QStringList parts = splitBox(s);
    QString directive = parts.at(0);

    if (isAbsolute()) {
        directive = "self";
        _current.normalisedContext = Box::currentRoot();
    }
    else if (directive.isEmpty()) {
        directive = "selfordescendants";
//        directive = "descendants";
        _current.normalisedContext = Box::currentRoot();
    }
    else {
        _current.normalisedContext = _originalContext;
    }

    return directive + ":" + parts.at(1) + "{" + parts.at(2) + "}";
}

QString Path::normaliseBox(QString s) {
    QStringList parts = splitBox(s);
    QString directive = parts.at(0);
    if (directive.isEmpty())
        directive = "children";
    return directive + ":" + parts.at(1) + "{" + parts.at(2) + "}";
}

QStringList Path::splitBox(QString s) {
    // Split
    int colon = s.indexOf(':'),
        leftBrace = s.indexOf('{'),
        rightBrace = s.indexOf('}');
    bool hasDirective = (colon>-1),
         hasType = (leftBrace>-1);
    QString directive = hasDirective ? s.left(colon) : "",
            box = s.mid(colon+1, leftBrace-colon-1),
            type = hasType ? s.mid(leftBrace+1, rightBrace-leftBrace-1) : "Box";

    // Check
    if (hasDirective) parseDirective(directive);

    // Normalise dots
    bool isSelf = (box == "."),
         isParent = (box == ".."),
         isNearest = (box == "...");
    if (isSelf)
        directive = "self";
    else if (isParent)
        directive = "parent";
    else if (isNearest)
        directive = "nearest";
    if (isSelf || isParent || isNearest) {
        if (hasDirective)
            throw Exception("Dot notation cannot follow directive", _current.originalPath, _caller);
        box = "*";
    }

    // Combine
    return QStringList() << directive << box << type;
}

QString Path::normalisePort() {
    int leftBracket = _current.originalPath.indexOf('['),
        rightBracket = _current.originalPath.indexOf(']');
    if (leftBracket*rightBracket < 0)
        throw Exception("Missing matching bracket", _current.originalPath, _originalContext);
    if (leftBracket == -1) return "";
    return "children:" +
            _current.originalPath.mid(leftBracket+1, rightBracket-leftBracket-1) +
            "{Port}";
}

QObjectList Path::nearest(QObject *p, QString tail) {
    Path path(tail, p);
    QObjectList candidates = path._resolve();
    if (candidates.isEmpty()) {
        QObject *parent = p->parent();
        if (parent)
            return nearest(parent, tail);
    }
    return candidates;
}

namespace {

    inline bool matches(QObject* candidate, QString box, QString type) {
        QStringList types = classInheritance(candidate).split("/");
        return (box=="*" || candidate->objectName()==box) && types.contains(type);
    }

    void findAncestors(QObject *p, QList<QObject*> &ancestors) {
        QObject *parent = p->parent();
        if (parent)
            findAncestors(parent, ancestors << parent);
    }

    QList<QObject*> allSiblings(QObject *p) {
        QObject *parent = p->parent();
        return (parent) ? parent->children() : QList<QObject*>();
    }

    QList<QObject*> otherSiblings(QObject *p) {
        QObject *parent = p->parent();
        QList<QObject*> sib;
        if (parent) {
            for (QObject *child : parent->children()) {
                if (child != p) sib << child;
            }
        }
        return sib;
    }

    QObject* preceedingSibling(QObject *p) {
        QObject *parent = p->parent();
        QObject *sib{0};
        if (parent) {
            for (QObject *child : parent->children()) {
                if (child == p) break;
                sib = child;
            }
        }
        return sib;
    }

    QObject* followingSibling(QObject *p) {
        QObject *parent = p->parent();
        QObject *sib{0};
        if (parent) {
            for (QObject *child : parent->children()) {
                if (sib == p) return child;
                sib = child;
            }
        }
        return 0;
    }

    QList<QObject*> filter(QList<QObject*> candidates, QString box, QString type) {
        QObjectList filtered;
        for (QObject *candidate : candidates) {
            if (matches(candidate, box, type))
                filtered << candidate;
        }
        return filtered;
    }

    QList<QObject*> filter(QObject *candidate, QString box, QString type) {
        return filter(QList<QObject*>() << candidate, box, type);
    }

} // namespace

void Path::addCandidates(QString path, QObjectList &candidates) {
    // Split
    int slash = path.indexOf('/');
    bool hasTail = (slash > -1),
         isNearest = false;
    QString head = path.left(slash),
            tail = hasTail ? path.mid(slash+1, -1) : "";
    QStringList parts = splitBox(head);
    QString directive = parts.at(0),
            box = parts.at(1),
            type = parts.at(2);

    QObjectList newCandidates;
    for (QObject *candidate : candidates) {
        if (!candidate) continue;
        QObjectList ancestors;
        QString findBoxName = (box == "*") ? QString() : box;
        switch (parseDirective(directive)) {
        case Self:
            newCandidates << filter(candidate, box, type);
            break;
        case Children:
            newCandidates << filter(candidate->findChildren<QObject*>(findBoxName, Qt::FindDirectChildrenOnly), box, type);
            break;
        case Parent:
            newCandidates << filter(candidate->parent(), box, type);
            break;
        case Nearest:
            newCandidates << nearest(candidate, tail);
            isNearest = true;
            break;
        case SelfOrDescendants:
            newCandidates << filter(candidate, box, type);
            // no break
        case Descendants:
            newCandidates << filter(candidate->findChildren<QObject*>(findBoxName, Qt::FindChildrenRecursively), box, type);
            break;
        case Ancestors:
            findAncestors(candidate, ancestors);
            newCandidates << filter(ancestors, box, type);
            break;
        case AllSiblings:
            newCandidates << filter(allSiblings(candidate), box, type);
            break;
        case OtherSiblings:
            newCandidates << filter(otherSiblings(candidate), box, type);
            break;
        case PreceedingSibling:
            newCandidates << filter(preceedingSibling(candidate), box, type);
            break;
        case FollowingSibling:
            newCandidates << filter(followingSibling(candidate), box, type);
            break;
        }
    }
    candidates = newCandidates;

    // For nearest objects, shorten list to the very nearest
    if (isNearest) {
        for (QObject *candidate : candidates) {
            if (candidate) {
                candidates.clear();
                candidates << candidate;
                break;
            }
        }
    }
    // For all others, resolve tail
    else if (hasTail)
        addCandidates(tail, candidates);
}

void Path::removeEmptyCandidates() {
    QObjectList filtered;
    for (QObject *candidate : _candidates) {
        if (candidate)
            filtered << candidate;
    }
    _candidates = filtered;
}

Path::Directive Path::parseDirective(QString s) {
    QString dir = s.toLower();
    if (!_directives.contains(dir))
        throw Exception("Unknown directive: '" + s + "'", _current.originalPath, _caller);
    return _directives.value(dir);
}

bool Path::isAbsolute() {
    return (!_current.originalPath.isEmpty() &&
            _current.originalPath.at(0) == '/');
}

} //namespace
