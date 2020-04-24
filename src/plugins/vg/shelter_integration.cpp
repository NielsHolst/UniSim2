/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <functional>
#include <boost/numeric/odeint.hpp>
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>

#include "shelter_integration.h"

using namespace base;
using namespace boost::numeric::odeint;
using namespace phys_math;

namespace vg {

PUBLISH(ShelterIntegration)

ShelterIntegration::ShelterIntegration(QString name, QObject *parent)
    : Box(name, parent)
{
    help("integrates glass+screen thermal flux");
    Input(Cglass).equals(8.64);
    Input(Cscreen).equals(1.44);
    Input(e_glass).equals(0.9);
    Input(e_screen_up).equals(0.4);
    Input(e_screen_down).equals(0.9);
    Input(e_indoors).equals(0.9);
    Input(c1).equals(7.2);
    Input(c2).equals(3.8);
    Input(c3).equals(1.247);
    Input(k).equals(195);
    Input(W).equals(4);
    Input(To).equals(0);
    Input(Ti).equals(20);
    Input(Tsky).equals(-10);
    // Fluxes to glass outer side
    Output(Qcond_to_go_from_gi);
    Output(Qconv_to_go_from_o);
    Output(Qradi_to_go_from_sky);
    // Fluxes to glass inner side
    Output(Qcond_to_gi_from_go);
    Output(Qconv_to_gi_from_i);
    Output(Qradi_to_gi_from_s);
    // Fluxes to screen
    Output(Qconv_to_s_from_i);
    Output(Qradi_to_s_from_gi);
    Output(Qradi_to_s_from_i);
    // Total fluxes
    Output(Qgo);
    Output(Qgi);
    Output(Qs);
    Output(eqTgo);
    Output(eqTgi);
    Output(eqTs);
    Output(steps);
}

void ShelterIntegration::initialize() {
}

void ShelterIntegration::reset() {
    double Tavg = (Ti+To)/2.;
    state =  State{Tavg, Tavg, Tavg};
    update();
}

// Wrap model method in a functor
struct Model  {
    Model(ShelterIntegration *si ) : _si(si) { }
    ShelterIntegration *_si;

    void operator()(ShelterIntegration::State const &x,
                    ShelterIntegration::State &dxdt,
                    const double t ) const
    {
        _si->model(x, dxdt, t);
    }
};

void ShelterIntegration::update() {
    Model model(this);
    steps = integrate(model, state, 0., 10., 0.01);
    eqTgo = state[0];
    eqTgi = state[1];
    eqTs  = state[2];
}

inline double qr(double x) { return pow(fabs(x), 1./3.); }

void ShelterIntegration::model(const State &x , State &dx , const double /* t */) {
  const double
    // State
    &Tgo(x[0]),
    &Tgi(x[1]),
    &Ts (x[2]);

  // Fluxes to glass outer side
  Qcond_to_go_from_gi = k*(Tgi-Tgo);
  Qconv_to_go_from_o  = (c1 + c2*W)*(To-Tgo);
  Qradi_to_go_from_sky  = Sigma*(-e_glass*p4K(Tgo) + p4K(Tsky));
  // Fluxes to glass inner side
  Qcond_to_gi_from_go = -Qcond_to_go_from_gi;
  Qconv_to_gi_from_i  = c3*qr(Ti-Tgi)*(Ti-Tgi);
  Qradi_to_gi_from_s  = Sigma*(1/(1/e_glass + 1/e_screen_up- 1))*(-p4K(Tgi) + p4K(Ts));
  // Fluxes to screen
  Qconv_to_s_from_i  = 2*c3*qr(Ti-Ts)*(Ti-Ts);
  Qradi_to_s_from_gi = -Qradi_to_gi_from_s;
  Qradi_to_s_from_i  = Sigma*(1/(1/e_indoors + 1/e_screen_down - 1))*(-p4K(Ts) + p4K(Ti));
  // Total fluxes
  Qgo = Qcond_to_go_from_gi + Qconv_to_go_from_o + Qradi_to_go_from_sky;
  Qgi = Qcond_to_gi_from_go + Qconv_to_gi_from_i + Qradi_to_gi_from_s;
  Qs  = Qconv_to_s_from_i + Qradi_to_s_from_gi + Qradi_to_s_from_i;
    // Result
  dx[0] = Qgo/Cglass;
  dx[1] = Qgi/Cglass;
  dx[2] = Qs /Cscreen;
}


} //namespace

