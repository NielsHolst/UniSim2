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
#include <QRegExp>
#include <QStringList>

namespace boxes {

class Box;

class Path {
public:
    Path(QString path, QObject *context = 0);
    QString normalise();
    QObjectList resolve();
    void validateName(QString name);
    void validateStep(QString step);
private:
    // Data
    QString _originalPath, _normalisedPath;
    QObject *_context, *_normalisedContext;
    QList<QObject*> _candidates;

    enum Directive {
        Self, Children, Parent, Nearest,
        Descendants, Ancestors,
        AllSiblings, OtherSiblings, PreceedingSibling, FollowingSibling};
    static QMap<QString, Directive> _directives;

    // Methods
    void validate(QRegExp rx, QString s);
    QString normaliseFirstBox(QString s);
    QString normaliseBox(QString s);
    QStringList splitBox(QString s);
    QString normalisePort();
    QObject* findContext();
    void addCandidates(QString path);
    void removeEmptyCandidates();
    Directive parseDirective(QString s);
    bool isAbsolute();
};

}

#endif
