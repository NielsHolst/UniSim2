/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "boundary_layer_resistance_base.h"
#include <base/publish.h>

using namespace base;

namespace vg {

/*! \class BoundaryLayerResistanceBase
 * \brief Boundary layer resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * Inputs
 * ------
 * - _leafDimension_ describes leaf geometry [mm]
 * - _indoorsWindSpeed_ is the wind speed indoors [m/s]
 *
 * Outputs
 * ------
 * - _rbH2O_ is the boundary layer resistance against water vapour [s/m]
 * - _rbCO2_ is the boundary layer resistance against CO<SUB>2</SUB> [s/m]
 */

BoundaryLayerResistanceBase::BoundaryLayerResistanceBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(BoundaryLayerResistanceBase);
    Input(leafDimension).equals(25./1000.);
    Input(leafWindSpeed).imports("../windSpeed[value]");
    Output(rbH2O);
    Output(rbCO2);
}

void BoundaryLayerResistanceBase::reset() {
    update();
}

void BoundaryLayerResistanceBase::update() {
    setRbH2O();
    rbCO2 = rbH2O*1.37;
}


} //namespace

