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

namespace boxes {

QMap<QString, Path::Directive> Path::_directives;

#define DIR(x) _directives[QString(#x).toLower()] = x

Path::Path(QString path, QObject *context)
    : _originalPath(path.trimmed()), _context(context)
{
    if (_directives.isEmpty()) {
        DIR(Self);
        DIR(Children);
        DIR(Parent);
        DIR(Nearest);
        DIR(Descendants);
        DIR(Ancestors);
        DIR(AllSiblings);
        DIR(OtherSiblings);
        DIR(PreceedingSibling);
        DIR(FollowingSibling);
    }
}

QObjectList Path::resolve() {
    _normalisedPath = normalise();
    _candidates.clear();
    _candidates << _normalisedContext;
    if (_normalisedContext == 0)
        throw Exception("Path misses a context object", _originalPath);
    addCandidates(_normalisedPath);
    removeEmptyCandidates();
    return _candidates;
}

QString Path::normalise() {
    QString result;
    int leftBracket = _originalPath.indexOf('[');
    QStringList boxes = _originalPath.left(leftBracket).split('/');

    if (isAbsolute()) {
        boxes.removeAt(0);
        if (boxes.at(0).isEmpty()) boxes[0] = "*";
    }

    result = normaliseFirstBox(boxes.at(0));
    for (int i = 1; i < boxes.size(); ++i)
        result += "/" + normaliseBox(boxes.at(i));

    QString port = normalisePort();
    if (!port.isEmpty())
        result += "/" + port;

    return result;
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
        throw Exception("Bad path format", value.arg(s).arg(_originalPath), _context);
}

QString Path::normaliseFirstBox(QString s) {
    QStringList parts = splitBox(s);
    QString directive = parts.at(0);

    if (isAbsolute()) {
        directive = "self";
        _normalisedContext = Box::currentRoot();
    }
    else if (directive.isEmpty()) {
        directive = "descendants";
        _normalisedContext = Box::currentRoot();
    }
    else {
        _normalisedContext = _context;
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
            throw Exception("Dot notation cannot follow directive", _originalPath);
        box = "*";
    }

    // Combine
    return QStringList() << directive << box << type;
}

QString Path::normalisePort() {
    int leftBracket = _originalPath.indexOf('['),
        rightBracket = _originalPath.indexOf(']');
    if (leftBracket*rightBracket < 0)
        throw Exception("Missing matching bracket", _originalPath, _context);
    if (leftBracket == -1) return "";
    return "children:" +
            _originalPath.mid(leftBracket+1, rightBracket-leftBracket-1) +
            "{Port}";
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

    QObjectList nearest(QObject *p, QString tail) {
        Path path(tail, p);
        QObjectList candidates = path.resolve();
        if (candidates.isEmpty()) {
            QObject *parent = p->parent();
            if (parent)
                return nearest(parent, tail);
        }
        return candidates;
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

//    QObject* filterFirst(QList<QObject*> candidates, QString box, QString type) {
//        for (QObject *candidate : candidates) {
//            if (matches(candidate, box, type))
//                return candidate;
//        }
//        return 0;
//    }

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

void Path::addCandidates(QString path) {
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
    for (QObject *candidate : _candidates) {
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
    _candidates = newCandidates;

    // For nearest objects, shorten list to the very nearest
    if (isNearest) {
        for (QObject *candidate : _candidates) {
            if (candidate) {
                _candidates.clear();
                _candidates << candidate;
                break;
            }
        }
    }
    // For all others, resolve tail
    else if (hasTail)
        addCandidates(tail);
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
        throw Exception("Unknown directive: '" + s + "'", _originalPath);
    return _directives.value(dir);
}

bool Path::isAbsolute() {
    return (_originalPath.at(0) == '/');
}

} //namespace
