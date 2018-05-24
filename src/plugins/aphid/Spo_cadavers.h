#ifndef SPO_CADAVERS_H
#define SPO_CADAVERS_H
#include <base/box.h>

namespace aphid {

class Spo_cadavers : public base::Box
{
public:
    Spo_cadavers(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double nb_cadavers_ready;
    bool Ticking;
    // Outputs
    bool nb_spo_cadavers;

};

}

#endif
