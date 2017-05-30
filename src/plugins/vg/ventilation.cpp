/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <math.h>
#include <base/box_builder.h>
#include <base/exception.h>
#include <base/path.h>
#include <base/publish.h>
#include "ventilation.h"

using namespace base;

namespace vg {
	
PUBLISH(Ventilation)

/*! \class Vents
 * \brief Calculates air exchange through vents
 *
 * Inputs
 * ------
 * - _groundArea_ is the area covered by the greenhouse [m<SUP>2</SUP>]
 *
 * Outputs
 * ------
 * - _totalLength_ is the total length of all vents summed [m]
 * - _averageHeight_ is the average height of all vents [m]
 * - _proportionalEffectiveArea_ is the total effective ventilation area per ground area [m<SUP>2</SUP> ventilation/m<SUP>2</SUP> ground]
 */

Ventilation::Ventilation(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(groundArea).imports("geometry[groundArea]");
    Output(totalLength);
    Output(averageHeight);
    Output(proportionalEffectiveArea);
}

void Ventilation::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./*"))
        builder.
        box("Windows").name("windows").
        endbox();
}

void Ventilation::reset() {
    totalLength =
    averageHeight =
    proportionalEffectiveArea = 0;
    QVector<Box*> allWindows = findMany<Box>("./*<Windows>");
    for (Box *windows : allWindows) {
        double length = windows->port("length")->value<double>(),
               height = windows->port("height")->value<double>();
        totalLength += length;
        averageHeight += height*length;
        proportionalEffectiveArea += windows->port("effectiveArea")->value<double>();
    }
    if (totalLength>0.) averageHeight /= totalLength;
    proportionalEffectiveArea /= groundArea;
    if (proportionalEffectiveArea > 1.)
        proportionalEffectiveArea = 1.;
}


} //namespace


