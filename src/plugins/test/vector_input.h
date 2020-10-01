#ifndef VECTOR_INPUT_H
#define VECTOR_INPUT_H
#include <QVector>
#include <base/box.h>

namespace test {

class VectorInput : public base::Box
{
public:
    VectorInput(QString name, QObject *parent);
private:
    QVector<int> numbers1, numbers2;
};

}

#endif
