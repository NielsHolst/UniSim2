/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "boundary_layer_resistance_monteith_unsworth.h"
#include <base/publish.h>

using namespace std;
using namespace base;

namespace vg {

PUBLISH(BoundaryLayerResistanceMonteithUnsworth)

/*! \class BoundaryLayerResistance
 * \brief Boundary layer resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * According to Monteith and Unsworth (1990).
 */

BoundaryLayerResistanceMonteithUnsworth::BoundaryLayerResistanceMonteithUnsworth(QString name, QObject *parent)
        : BoundaryLayerResistanceBase(name, parent)
{
    Input(leafTemperature).imports("../temperature[value]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Output(gbH2Oforced);
    Output(gbH2Ofree);
}

void BoundaryLayerResistanceMonteithUnsworth::reset() {
    leafTemperature = indoorsTemperature;
    BoundaryLayerResistanceBase::reset();
}

void BoundaryLayerResistanceMonteithUnsworth::setRbH2O() {
    // Forced and free resistances are considered in parallel (i.e. as alternate pathways for water vapour)
    // Hence total resistance is found through addition of conductances
    double dTemp = fabs(leafTemperature - indoorsTemperature);
    gbH2Oforced = 1./( 300*sqrt(leafDimension/leafWindSpeed) );
    gbH2Ofree = 1./( 1000*pow(leafDimension/dTemp, 0.25) );
    rbH2O = 1./(gbH2Oforced + gbH2Ofree);
}

} //namespace

