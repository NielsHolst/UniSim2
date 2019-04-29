#ifndef CADAVERS_TIME_H
#define CADAVERS_TIME_H
#include <base/box.h>

namespace aphid {

class CadaverTime : public base::Box
{
public:
    CadaverTime(QString name, QObject *parent);
    void update();
private:
    // Inputs
    bool isSporulating;
    double timeStep, rhAccelaration;
    // Outputs
    double step, total;
};

}

#endif
