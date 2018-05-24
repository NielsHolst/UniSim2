#include "track.h"
#include "track_collection.h"

namespace base {

TrackCollection::TrackCollection(QString name, QObject *parent)
    : QObject(parent)
{
    setObjectName(name);
}

void TrackCollection::clear() {
    _tracks.clear();
}

void TrackCollection::append(base::Track *track) {
    _tracks << track;
}

bool TrackCollection::isEmpty() const {
    return _tracks.isEmpty();
}

const QVector<base::Track*> & TrackCollection::tracks() {
    return _tracks;
}

} //namespace


