/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_TRACK_COLLECTION_H
#define BASE_TRACK_COLLECTION_H

#include <QObject>
#include <QVector>

namespace base {

class Track;

class TrackCollection : public QObject
{
public:
    TrackCollection(QString name, QObject *parent);
    void clear();
    void append(base::Track *track);
    bool isEmpty() const;
    const QVector<base::Track*> & tracks();
private:
    QVector<base::Track*> _tracks;
};



} //namespace

#endif

