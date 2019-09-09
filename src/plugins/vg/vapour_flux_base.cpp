/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "vapour_flux_base.h"

using namespace base;

namespace vg {

VapourFluxBase::VapourFluxBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(VapourFluxBase);
    help("base class for vapour flux computations");
    Output(conductance).help("Vapour flux conductance").unit("s/m");
    Output(vapourFlux).help("Vapour flux rate").unit("kg/m2/s");
    Output(gain).help("For integration of all vapour fluxes (GCC, p. 148)").unit("kg/m2/s");
}

} //namespace

