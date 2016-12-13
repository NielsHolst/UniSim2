/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <algorithm>
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

Path::Path(const QObject *context)
    : _originalContext(context), _caller(0)
{
    initDirectives();
}

Path::Path(QString path, const QObject *context)
    : Path(context)
{
    _originalPaths << path.split("|", QString::SkipEmptyParts);
}

Path::Path(QStringList paths, const QObject *context)
    : Path(context)
{
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

Path::QObjects Path::_resolve(int number, const QObject *caller) {
    _caller = caller;
    _candidates.clear();
    _current.normalisedContext = 0;
    for (int i = 0; i <_originalPaths.size(); ++i) {
        normalise(i);
        if (_current.normalisedContext == 0)
            ThrowException("Path misses a context object").value(_current.originalPath).context(_caller);
        QObjects nextCandidates = (QObjects() << _current.normalisedContext);
        addCandidates(_current.normalisedPath, nextCandidates);
        _candidates << nextCandidates;
    }
    removeEmptyCandidates();
    if (number > -1 && _candidates.size() != number) {
        QString msg{"Path resolves to the wrong number of mathes: found(%1), expected(%2)"};
        ThrowException(msg.arg(_candidates.size()).arg(number)).value(_current.originalPath).context(_caller);
    }
//    sort(_candidates.begin(), _candidates.end(),
//         [](Port *a, Port *b) { return a->objectName() < b->objectName(); });

    return _candidates;
}

inline bool hasDirective(QString s) { return s.indexOf(':') > -1; }

QString Path::normalise(int ix) {
     _current.originalPath = _originalPaths.at(ix).simplified();
     _current.originalPath.remove(QChar(' '));
     _current.normalisedContext = _originalContext;

    int leftBracket = _current.originalPath.indexOf('[');
    QStringList base = _current.originalPath.left(leftBracket).split('/');

    if (base.isEmpty())
        ThrowException("Path invalid or empty").value(_current.originalPath).context(_caller);

    if (isAbsolute()) {
        base.removeAt(0);
        if (base.at(0).isEmpty()) base[0] = "*";
    }

    // We need special treatment for triple-dots
    QString result;
    int next = 1;
    if (_current.originalPath.startsWith("...[")) {
        if (base.size() == 1)
            return _current.normalisedPath = normalisePort("nearest");
        else
            ThrowException("Illegal path format").value(_current.originalPath).context(_caller);
    }
    if (base.at(0) == "...") {
        if (base.size() == 1)
            ThrowException("Illegal path format").value(_current.originalPath).context(_caller);
        result = normaliseBox("nearest", base.at(1));
        next = 2;
    }
    else {
        result = normaliseFirstBox(base.at(0));
    }

    for (int i = next; i < base.size(); ++i)
        result += "/" + normaliseBox("children", base.at(i));

    QString port = normalisePort("children");
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
        ThrowException("Bad path format").value(value.arg(s).arg(_current.originalPath)).context(_originalContext);
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
        _current.normalisedContext = Box::currentRoot();
    }
    else {
        _current.normalisedContext = _originalContext;
    }

    return directive + ":" + parts.at(1) + "<" + parts.at(2) + ">";
}

QString Path::normaliseBox(QString defaultDirective, QString s) {
    QStringList parts = splitBox(s);
    QString directive = parts.at(0);
    if (directive.isEmpty())
        directive = defaultDirective;
    return directive + ":" + parts.at(1) + "<" + parts.at(2) + ">";
}

QStringList Path::splitBox(QString s) {
    // Accept double-colons as single-colons
    s.replace("::", ":");
    // Split
    int colon = s.indexOf(':'),
        leftBrace = s.indexOf('<'),
        rightBrace = s.indexOf('>');
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
            ThrowException("Dot notation cannot follow directive").value(_current.originalPath).context(_caller);
        box = "*";
    }
    if (isNearest)
        ThrowException("Triple-dots only allowed at path beginning").value(_current.originalPath).context(_caller);
    // Combine
    return QStringList() << directive << box << type;
}

QString Path::normalisePort(QString directive) {
    int leftBracket = _current.originalPath.indexOf('['),
        rightBracket = _current.originalPath.indexOf(']');
    if (leftBracket*rightBracket < 0)
        ThrowException("Missing matching bracket").value(_current.originalPath).context(_originalContext);
    if (leftBracket == -1) return "";
    return directive + ":" +
            _current.originalPath.mid(leftBracket+1, rightBracket-leftBracket-1) +
            "<Port>";
}

Path::QObjects Path::nearest(const QObject *p, QString box, QString type, QString tail) {
    QString s;
    if (type == "Port")
        s = ".[" + box + "]" ;
    else if (tail.isEmpty())
        s = "./" + box;
    else
        s = "./" + box + "/" +tail;
    Path path(s, p);
    QObjects candidates = path._resolve();
    if (candidates.isEmpty()) {
        const QObject *parent = p->parent();
        if (parent)
            return nearest(parent, box, type, tail);
    }
    return candidates;
}

namespace {

    bool hasNamespace(QString s) {
        return s.contains("::");
    }

    void removeNamespace(QString &s) {
        int ix = s.indexOf("::");
        if (ix > -1)
            s = s.mid(ix+2);
    }

    void removeNamespace(QStringList &sl) {
        for (int i = 0; i < sl.size(); ++i)
            removeNamespace(sl[i]);
    }

    bool matches(const QObject* candidate, QString box, QString type) {
        if (!candidate)
            return false;
        QStringList types = classInheritance(candidate).split("/");
        if (!hasNamespace(box))
            removeNamespace(types);
        return (box=="*" || candidate->objectName()==box) && types.contains(type);
    }

    void findAncestors(const QObject *p, Path::QObjects &ancestors) {
        Q_ASSERT(p);
        QObject *parent = p->parent();
        if (parent)
            findAncestors(parent, ancestors << parent);
    }

    Path::QObjects allSiblings(const QObject *p) {
        Q_ASSERT(p);
        const QObject *parent = p->parent();
        return (parent) ? parent->findChildren<const QObject*>(QString(), Qt::FindDirectChildrenOnly)
                        : Path::QObjects();
    }

    Path::QObjects otherSiblings(const QObject *p) {
        Q_ASSERT(p);
        const QObject *parent = p->parent();
        Path::QObjects sib;
        if (parent) {
            for (const QObject *child : parent->children()) {
                if (child != p) sib << child;
            }
        }
        return sib;
    }

    const QObject* preceedingSibling(const QObject *p) {
        Q_ASSERT(p);
        const QObject *parent = p->parent();
        const QObject *sib{0};
        if (parent) {
            for (const QObject *child : parent->children()) {
                if (child == p) break;
                sib = child;
            }
        }
        return sib;
    }

    const QObject* followingSibling(const QObject *p) {
        Q_ASSERT(p);
        const QObject *parent = p->parent();
        const QObject *sib{0};
        if (parent) {
            for (const QObject *child : parent->children()) {
                if (sib == p) return child;
                sib = child;
            }
        }
        return 0;
    }

    Path::QObjects filter(Path::QObjects candidates, QString box, QString type) {
        Path::QObjects filtered;
        for (const QObject *candidate : candidates) {
            if (matches(candidate, box, type))
                filtered << candidate;
        }
        return filtered;
    }

    Path::QObjects filter(const QObject *candidate, QString box, QString type) {
        return filter(Path::QObjects() << candidate, box, type);
    }

} // namespace

void Path::addCandidates(QString path, QObjects &candidates) {
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

    if (type=="Port" && !tail.isEmpty())
        ThrowException("Illegal path; nothing can follow the port").value(_current.originalPath).context(_caller);

    QObjects newCandidates;
    for (const QObject *candidate : candidates) {
        if (!candidate) continue;
        QObjects ancestors;
        QString findBoxName = (box == "*") ? QString() : box;
        switch (parseDirective(directive)) {
        case Self:
            newCandidates << filter(candidate, box, type);
            break;
        case Children:
            newCandidates << filter(candidate->findChildren<const QObject*>(findBoxName, Qt::FindDirectChildrenOnly), box, type);
            break;
        case Parent:
            newCandidates << filter(candidate->parent(), box, type);
            break;
        case Nearest:
            newCandidates << nearest(candidate, box, type, tail);
            isNearest = true;
            break;
        case SelfOrDescendants:
            newCandidates << filter(candidate, box, type);
            // no break
        case Descendants:
            newCandidates << filter(candidate->findChildren<const QObject*>(findBoxName, Qt::FindChildrenRecursively), box, type);
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
        for (const QObject *candidate : candidates) {
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
    QObjects filtered;
    for (const QObject *candidate : _candidates) {
        if (candidate)
            filtered << candidate;
    }
    _candidates = filtered;
}

Path::Directive Path::parseDirective(QString s) {
    QString dir = s.toLower();
    if (!_directives.contains(dir))
        ThrowException("Unknown directive: '" + s + "'").value(_current.originalPath).context(_caller);
    return _directives.value(dir);
}

bool Path::isAbsolute() {
    return (!_current.originalPath.isEmpty() &&
            _current.originalPath.at(0) == '/');
}

} //namespace
