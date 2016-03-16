#ifndef BUFFER_H
#define BUFFER_H
#include <boost/circular_buffer.hpp>
#include <QVector>
#include <base/box.h>

namespace boxes {

class Buffer : public base::Box
{
public:
    Buffer(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double initial, input, increment;
    int size;
    // Outputs
    double sum, average;
    QVector<double> buffer;
    // Data
    bool _firstUpdate;
    boost::circular_buffer<double> _buffer;
    // Methods
    void updateOutputBuffer();
};

}

#endif
