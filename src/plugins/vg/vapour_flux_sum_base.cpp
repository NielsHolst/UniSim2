/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "vapour_flux_base.h"
#include "vapour_flux_sum_base.h"

using namespace base;

namespace vg {

VapourFluxSumBase::VapourFluxSumBase(QString name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    help("base class for summing vapour fluxes");
}

void VapourFluxSumBase::initialize() {
    ptrs.clear();
    for (auto flux : fluxes()) {
      const double *co = flux->port("conductance")->valuePtr<double>(),
                   *va = flux->port("vapourFlux")->valuePtr<double>(),
                   *ga = flux->port("gain")->valuePtr<double>();
      ptrs << Ptr{co, va, ga};
    }
}

void VapourFluxSumBase::update() {
    conductance = vapourFlux = gain = 0;
    for (auto p : ptrs) {
        conductance += *(p.conductance);
        vapourFlux += *(p.vapourFlux);
        gain += *(p.gain);
    }
}

} //namespace

