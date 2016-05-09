#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H
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
    // else puts the value where tail currently is
    void push(T value);
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
    // Works as 'at' but allows the value at the indes position to be changed
    T& operator[](int i);
private:
    // Data
    QVector<T> *_vector;
    int _size, _head, _tail;
    bool _full;
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
}

template <class T>
void CircularBuffer<T>::push(T value) {
    if (_full || _head == -1)
        _tail = (_tail + 1) % _size;
    _head = (_head + 1) % _size;
    (*_vector)[_head] = value;
    _full = (_full || _head == _size-1);
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
T& CircularBuffer<T>::operator[](int i) {
    return (*_vector)[ (_head + i) % _size ];
}

}

#endif
