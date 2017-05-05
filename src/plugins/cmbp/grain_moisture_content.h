#ifndef GRAIN_MOISTURE_CONTENT_H
#define GRAIN_MOISTURE_CONTENT_H
#include <base/box.h>

namespace cmbp {

class GrainMoistureContent : public base::Box
{
public:
    GrainMoistureContent(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double c1, c2, c3, rh;
    // Output
    double value;
};

}

#endif
