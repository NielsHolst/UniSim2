#ifndef AREA_H
#define AREA_H
#include <base/box.h>

namespace saccharina {

class Area : public base::Box
{
public:
    Area(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double u, Ainit, fronderosion, C, Cmin, Cstruct, Alost, Factor;
    // Outputs
    double A;
};

}

#endif
