#ifndef CADAVERDENSITY_H
#define CADAVERDENSITY_H
#include <base/box.h>

namespace aphid {

class CadaverDensity : public base::Box
{
public:
    CadaverDensity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double nb_cadavers; // for now cadaver units
    double nb_plant;
    // Outputs
    double d_cadaver;

};

}

#endif
