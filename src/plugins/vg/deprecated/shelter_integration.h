/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SHELTER_INTEGRATION_H
#define SHELTER_INTEGRATION_H
#include <vector>
#include <base/box.h>

namespace vg {

class ShelterIntegration : public base::Box
{
public: 
    ShelterIntegration(QString name, QObject *parent=0);
    void initialize();
    void reset();
    void update();
    typedef std::vector< double > State;
    void model(const State &x , State &dx , const double /* t */);
private:
    // Inputs
    double
        Cglass, Cscreen,
        e_glass, e_screen_up, e_screen_down, e_indoors,
        c1, c2, c3,
        k, W,
        To, Ti, Ts, Tsky;
    // Outputs
    double
        // Fluxes to glass outer side
        Qcond_to_go_from_gi,
        Qconv_to_go_from_o,
        Qradi_to_go_from_sky,
        // Fluxes to glass inner side
        Qcond_to_gi_from_go,
        Qconv_to_gi_from_i,
        Qradi_to_gi_from_s,
        // Fluxes to screen
        Qconv_to_s_from_i,
        Qradi_to_s_from_gi,
        Qradi_to_s_from_i,
        // Total fluxes
        Qgo,
        Qgi,
        Qs,
        eqTgo, eqTgi, eqTs, eqFlux;
    int steps;
    // Data
    State state;
};

} //namespace
#endif
