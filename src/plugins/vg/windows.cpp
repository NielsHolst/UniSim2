/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <math.h>
#include <base/exception.h>
#include <base/publish.h>
#include "general.h"
#include "windows.h"

using namespace base;

namespace vg {
	
PUBLISH(Windows)

Windows::Windows(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(roofPitch).imports("geometry[roofPitch]");
    Input(groundArea).imports("geometry[groundArea]");
    Input(windSpeed).imports("outdoors[windSpeed]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(state).help("Relative opening [0;1]");

    Input(orientation).equals("leeward").help("Either leeward or windward");
    Input(relativeArea).equals(0.1).help("Area of windows in proportion to ground area [0;1]");
    Input(length).equals(3.5).help("Length of one window (m)");
    Input(width).equals(0.8).help("Width of one window (m)");
    Input(transmissivity).equals(1.).help("Transmissivity across windows area [0;1]");
    Output(number).help("Number of windows");
    Output(verticalOpening).help("Vertical opening according to state (m)");
}

void Windows::reset() {
    number = relativeArea*groundArea/length/width;
    _maxVerticalOpening = sin(roofPitch*2/PI)*width;
    setWindVentilationParameters();
}
void Windows::setWindVentilationParameters() {
    // After De Jong (1990, page 46 and 58)
    double aspectRatio = length/width;
    if (aspectRatio < 0.47)
        aspectRatio = 0.47;
    else if (aspectRatio > 1.825)
        aspectRatio = 1.825;
    if (orientation == "leeward") {
        _windVentilationMax = 0.0203 + 0.002212*aspectRatio;
        _windVentilationSlope = 1.006 + 11.47*aspectRatio;
    }
    else if (orientation == "windward") {
        _windVentilationMax = 0.0203 + 0.002212*aspectRatio;
        _windVentilationSlope = 1.006 + 11.47*aspectRatio;

    }
}

void Windows::update() {
    verticalOpening = state*_maxVerticalOpening;
}

} //namespace


