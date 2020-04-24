#ifndef DAY_LENGTH_SCALE_H
#define DAY_LENGTH_SCALE_H
#include <base/box.h>

namespace saccharina {

class DayLengthScale : public base::Box
{
public:
    DayLengthScale(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dayLength, valueToScale;
    // Outputs
    double value;
};

}

#endif
