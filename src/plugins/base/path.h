/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PATH_H
#define BASE_PATH_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QStringList>
#include <QVector>
#include "exception.h"

namespace base {

class Box;

class Path {
public:
    Path(QString path, const QObject *context = nullptr);
    Path(QStringList paths, const QObject *context = nullptr);
    QString normalise(int ix = 0);
    void validateName(QString name);
    void validateStep(QString step);
    template<class T=QObject> T* resolveOne(const QObject* caller);
    template<class T=QObject> T* resolveMaybeOne(const QObject* caller = nullptr);
    template<class T=QObject> QVector<T*> resolveMany(const QObject* caller = nullptr);

    typedef QList<const QObject*> QObjects;
private:
    // Data
    QStringList _originalPaths;
    const QObject *_originalContext, *_caller;
    struct {
        QString originalPath, normalisedPath;
        const QObject *normalisedContext;
    } _current;
    QObjects _candidates;

    enum Directive {
        Self, Children, Parent, Nearest,
        SelfOrDescendants, Descendants, Ancestors,
        AllSiblings, OtherSiblings, PreceedingSibling, FollowingSibling};
    static QMap<QString, Directive> _directives;

    // Methods
    Path(const QObject *context);
    void initDirectives();
    template<class T=QObject> QVector<T*> resolve(int number = -1, const QObject* caller = nullptr);
    QObjects _resolve(int number = -1, const QObject* caller = nullptr);
    void validate(QRegExp rx, QString s);
    QString normaliseFirstBox(QString s);
    QString normaliseBox(QString defaultDirective, QString s);
    QStringList splitBox(QString s);
    QString normalisePort(QString directive);
    QObjects nearest(const QObject *p, QString box, QString type, QString tail);
    QObject* findContext();
    void addCandidates(QString path, QObjects &candidates);
    void removeEmptyCandidates();
    void removeDuplicateCandidates();
    Directive parseDirective(QString s);
    bool isAbsolute();
};

template<class T> QVector<T*> Path::resolve(int number, const QObject* caller) {
    QObjects objs = _resolve(number, caller);
    QVector<T*> objsT;
    for (const QObject *obj : objs) {
        const T *objT = dynamic_cast<const T*>(obj);
        if (objT)
            objsT << const_cast<T*>(objT);
    }
    return objsT;
}

template<class T> T* Path::resolveOne(const QObject* caller) {
    return resolve<T>(1,caller).at(0);
}

template<class T> T* Path::resolveMaybeOne(const QObject* caller) {
    QVector<T*> objsT = resolve<T>(-1,caller);
    switch (objsT.size()) {
    case 0: return 0;
    case 1: return objsT.at(0);
    }
    QString msg{"Path resolves to the wrong number of mathes: found(%1), expected(0 or 1)"};
    ThrowException(msg.arg(objsT.size())).value(_current.originalPath).context(_caller);
}

template<class T> QVector<T*> Path::resolveMany(const QObject *caller) {
    return resolve<T>(-1, caller);
}

}

#endif
