/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "air_flux_base.h"

using namespace base;

namespace vg {

AirFluxBase::AirFluxBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(AirFluxBase);
    help("computes air exchange rate");
    Output(value).help("Relative rate of air exchange").unit("m3/m3/h");
}

} //namespace

