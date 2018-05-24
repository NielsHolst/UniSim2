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

