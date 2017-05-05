#ifndef FECUNDITY_H
#define FECUNDITY_H
#include <base/box.h>

namespace cmbp {

class Fecundity : public base::Box
{
public:
    Fecundity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double maxFecundity, r, T, sexRatio;
    // Output
    double value;
};

}

#endif
