/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PATH_H
#define PATH_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QStringList>
#include <QVector>

namespace boxes {

class Box;

class Path {
public:
    Path(QString path, QObject *context = 0);
    Path(QStringList paths, QObject *context = 0);
    QString normalise(int ix = 0);
    void validateName(QString name);
    void validateStep(QString step);
    template<class T=QObject> QVector<T*> resolve(int number = -1, QObject* caller = 0);
    template<class T=QObject> T* resolveOne(QObject* caller = 0);
    template<class T=QObject> T* resolveMaybeOne(QObject* caller = 0);
private:
    // Data
    QStringList _originalPaths;
    QObject *_originalContext, *_caller;
    struct {
        QString originalPath, normalisedPath;
        QObject *normalisedContext;
    } _current;
    QObjectList _candidates;

    enum Directive {
        Self, Children, Parent, Nearest,
        SelfOrDescendants, Descendants, Ancestors,
        AllSiblings, OtherSiblings, PreceedingSibling, FollowingSibling};
    static QMap<QString, Directive> _directives;

    // Methods
    void initDirectives();
    QObjectList _resolve(int number = -1, QObject* caller = 0);
    void validate(QRegExp rx, QString s);
    QString normaliseFirstBox(QString s);
    QString normaliseBox(QString s);
    QStringList splitBox(QString s);
    QString normalisePort();
    QObjectList nearest(QObject *p, QString tail);
    QObject* findContext();
    void addCandidates(QString path, QObjectList &candidates);
    void removeEmptyCandidates();
    Directive parseDirective(QString s);
    bool isAbsolute();
};

template<class T> QVector<T*> Path::resolve(int number, QObject* caller) {
    QObjectList objs = _resolve(number, caller);
    QVector<T*> objsT;
    for (QObject *obj : objs) {
        T *objT = dynamic_cast<T*>(obj);
        if (objT)
            objsT << objT;
    }
    return objsT;
}

template<class T> T* Path::resolveOne(QObject* caller) {
    return resolve<T>(1,caller).at(0);
}

template<class T> T* Path::resolveMaybeOne(QObject* caller) {
    QVector<T*> objsT = resolve<T>(-1,caller);
    switch (objsT.size()) {
    case 0: return 0;
    case 1: return objsT.at(0);
    }
    QString msg{"Path resolves to the wrong number of mathes: found(%1), expected(0 or 1)"};
    throw Exception(msg.arg(objsT.size()), _current.originalPath, _caller);
}

}

#endif
