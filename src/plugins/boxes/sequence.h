#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <base/box.h>

namespace boxes {

class Sequence : public base::Box
{
public:
    Sequence(QString name, QObject *parent);
    void amend();
    void reset();
    void update();
private:
    // Inputs
    double min, max;
    QString by;
    // Outputs
    int counter, counterMax, offset;
    double value;
    // Methods
    void updateValue();
};

}

#endif
