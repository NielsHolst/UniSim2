#ifndef VECTOR_SUM_H
#define VECTOR_SUM_H
#include <QList>
#include <QVector>
#include <base/box.h>

namespace base {
    class Port;
}

namespace boxes {

class VectorSum : public base::Box
{
public:
    VectorSum(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    QString input;
    // Outputs
    QVector<double> value;
    // Data
    QList< const QVector<double>* > _inputPtrs;
    int _size;
};

}

#endif
