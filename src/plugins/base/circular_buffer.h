#ifndef BASE_CIRCULAR_BUFFER_H
#define BASE_CIRCULAR_BUFFER_H
#include <numeric>
#include <QVector>

namespace base {

template <class T>
class CircularBuffer
{
public:
    // Constructs and clears the cb
    CircularBuffer(QVector<T> *buffer);
    // Changes the buffer size and clears it
    void resize(int size);
    // Empties the cb
    void clear();
    // Puts a value at the next empty place in the buffer (if cb is not yet full),
    // else puts the value where tail currently is;
    // returns the value pushed out (lost) from the buffer
    T push(T value);
    // The cb is empty just after construction and after clear()
    bool isEmpty() const;
    // The cb is full if a value has been pushed size number of times
    bool isFull() const;
    // Head is the latest value pushed
    T head() const;
    // Tail is the oldest value pushed
    T tail() const;
    // Index of head in the buffer; equals -1 if buffer is empty
    int headIndex() const;
    // Returns value indexed relative to head (at index 0), i.e. and increasing index returns progressively older values
    T at(int i) const;
    // Sets the value at(i); returns the value lost from that position
    T set(int i, T value);
    // Sum of buffer contents
    T sum() const;
    // Average of buffer contents
    T average() const;
    // Minimum value of buffer contents
    T min() const;
    // Maximum value of buffer contents
    T max() const;
    // Re-calculate statistics
    void updateStatistics();
private:
    // Data
    QVector<T> *_vector;
    int _size, _head, _tail;
    bool _full;
    T _sum, _min, _max;
    // Methods
    T replace(T &place, T value);
    int sizeUsed() const;
};

template <class T>
CircularBuffer<T>::CircularBuffer(QVector<T> *buffer)
    : _vector(buffer), _size(buffer->size())
{
    clear();
}

template <class T>
void CircularBuffer<T>::resize(int size) {
    _vector->fill(T(), _size = size);
    clear();
}

template <class T>
void CircularBuffer<T>::clear() {
    _vector->fill(T());
    _head = _tail = -1;
    _full = false;
    _sum = 0;
    _min = std::numeric_limits<T>::max();
    _max = -std::numeric_limits<T>::max();
}

template <class T>
T CircularBuffer<T>::push(T value) {
    if (_full || _head == -1)
        _tail = (_tail + 1) % _size;

    _head = (_head + 1) % _size;
    _full = (_full || _head == _size-1);

    return replace( (*_vector)[_head], value );


//    T &place((*_vector)[_head]),
//      pushedOut = place;
//    _sum += value - place;
//    if (value < _min)
//        _min = value;
//    if (value > _max)
//        _max = value;
//    place = value;
//    _full = (_full || _head == _size-1);
//    return pushedOut;
}

template <class T>
T CircularBuffer<T>::replace(T &place, T value) {
    T pushedOut = place;
    _sum += value - place;
    if (value < _min)
        _min = value;
    if (value > _max)
        _max = value;
    place = value;
    return pushedOut;
}

template <class T>
bool CircularBuffer<T>::isEmpty() const {
    return _head == -1;
}

template <class T>
bool CircularBuffer<T>::isFull() const {
    return _full;
}

template <class T>
T CircularBuffer<T>::head() const {
    return _vector->at(_head);
}

template <class T>
T CircularBuffer<T>::tail() const {
    return _vector->at(_tail);
}

template <class T>
int CircularBuffer<T>::headIndex() const {
    return _head;
}

template <class T>
T CircularBuffer<T>::at(int i) const {
    return _vector->at( (_head + i) % _size );
}

template <class T>
T CircularBuffer<T>::set(int i, T value) {
    return replace( (*_vector)[(_head + i) % _size], value );
}

template <class T>
T CircularBuffer<T>::sum() const {
    return _sum;
}

template <class T>
T CircularBuffer<T>::average() const {
    int n = sizeUsed();
    return (n==0) ? 0 : _sum/n;
}

template <class T>
int CircularBuffer<T>::sizeUsed() const {
    return (_full || _head == -1) ? _size :(_head+1);
}

template <class T>
T CircularBuffer<T>::min() const {
    return _min;
}

template <class T>
T CircularBuffer<T>::max() const {
    return _max;
}

template <class T>
void CircularBuffer<T>::updateStatistics() {
    _sum = 0;
    _min = std::numeric_limits<T>::max();
    _max = -std::numeric_limits<T>::max();
    const T *p = _vector->data();
    int n = sizeUsed();
    for (int i = 0; i < n; ++i, ++p) {
        _sum += *p;
        if (*p < _min)
            _min = *p;
        if (*p > _max)
            _max = *p;
    }
}

}

#endif
