#ifndef INFECTION_PRESSURE_H
#define INFECTION_PRESSURE_H
#include <base/box.h>

namespace aphid {

class Infection_pressure : public base::Box
{
public:
    Infection_pressure(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double cadavers_N_apterous;
    double cadavers_A_apterous;
    double cadavers_N_alate;
    double density_aphid;
    double density_aphid_threshold;
    double sporulation_capacity;
    // Outputs
    double nb_sporulating_cadavers;
    double infection_capacity;
    // Data
    double cadavers_ready_to_sporulate_D1;
    double cadavers_ready_to_sporulate_D2;
    double cadavers_ready_to_sporulate_D3;
    double cadavers_ready_to_sporulate_D4;
    double cadavers_ready_to_sporulate_D5;
    double cadavers_ready_to_sporulate_D6;
    double cadavers_ready_to_sporulate_D7;
    double cadavers_ready_to_sporulate;
};

}

#endif
