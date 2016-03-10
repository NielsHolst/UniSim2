/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop_lai.h"

using namespace base;

namespace vg {

/*! \class CropLai
 * \brief Base class for crop leaf area index
 *
 * Inputs
 * ------
 * - _laiStartPerPlant_ is the LAI per plant when the crop is established [m<SUP>2</SUP> leaf/m<SUP>2</SUP> planted area]
 * - _fractionPlantArea_ is the fraction of ground area covered by the crop [0;1]
 *
 * Output
 * ------
 * - _lai_ is the crop leaf area index [m<SUP>2</SUP> leaf/m<SUP>2</SUP> planted area]
 *
 */

CropLai::CropLai(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(laiStartPerPlant).equals(0.3);
    Input(fractionPlantArea).equals(1.);
    Output(value);
}

void CropLai::reset() {
    value = laiStartPerPlant;
}

} //namespace

