/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <math.h>
#include <base/exception.h>
#include <base/publish.h>
#include "windows.h"

using namespace base;

namespace vg {
	
PUBLISH(Windows)

/*! \class Vent
 * \brief Represents a number of similar vents
 *
 * Inputs
 * ------
 * - _length_ is the length of a vent [m]
 * - _height_ is the height of a vent [m]
 * - _number_ is the number of vents [1,2,3,...]
 * - _porosity_ is the efficacy of ventilation (can be reduced, for example, by an insect net) [0;1]
 *
 * Outputs
 * ------
 * - _effectiveArea_ is the total area of the vents, corrected for porosity [m<SUP>2</SUP>]

 */

Windows::Windows(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(groundArea).imports("geometry[groundArea]");
    Input(relativeArea).equals(0.1);
    Input(length).equals(3.5);
    Input(height).equals(0.8);
    Input(porosity).equals(1.);
    Output(number);
    Output(effectiveArea);
}

void Windows::reset() {
    number = relativeArea*groundArea/length/height;
    effectiveArea = relativeArea*groundArea*porosity;
}

} //namespace


