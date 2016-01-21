#include <boxes/general.h>
#include "history.h"

using namespace boxes;

namespace console {

History::History(int bufferSize)
    : _bufferSize(bufferSize), _begin(-1), _end(bufferSize-2)
{
    _buffer.fill(QStringList(), _bufferSize);
}

void History::add(QString entry) {
    _buffer[inc(_begin)] = split(entry);
    inc(_end);
}

QStringList History::entry(int index) {
    return (_begin == -1) ? QStringList()
                          : _buffer.at((_begin + _bufferSize - index)%_bufferSize);
}

int History::inc(int &i) {
    return i = (i+1)%_bufferSize;
}

}
