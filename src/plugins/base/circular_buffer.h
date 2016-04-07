#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H
#include <QVector>

namespace base {

template <class T>
class CircularBuffer
{
public:
    CircularBuffer(QVector<T> *buffer);
    void resize(int size);
    void clear();
    void push(T value);
    bool isFull() const;
    T head() const;
    T tail() const;
    T at(int i) const;
    T& operator[](int i);
private:
    // Data
    QVector<T> *_vector;
    int _size;
    T _head, _tail;
    bool _full;
};

template <class T>
CircularBuffer<T>::CircularBuffer(QVector<T> *buffer)
    : _vector(buffer), _size(0)
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
T CircularBuffer<T>::at(int i) const {
    Q_ASSERT(i < _size);
    return _vector->at( (_head + i) % _size );
}

template <class T>
T& CircularBuffer<T>::operator[](int i) {
    Q_ASSERT(i < _size);
    return (*_vector)[ (_head + i) % _size ];
}

}

#endif
