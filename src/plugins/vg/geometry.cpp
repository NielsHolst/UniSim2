/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/exception.h>
#include <base/publish.h>
#include "geometry.h"
#include "general.h"

using namespace base;

namespace vg {
	
PUBLISH(Geometry)

Geometry::Geometry(QString name, QObject *parent)
	: Box(name, parent)
{
    help("defines the measures of the greenhouse");
    Input(numSpans).equals(1).help("Number of spans");
    Input(spanWidth).equals(20.).help("Width of a span (m)");
    Input(length).equals(50.).help("Length of side wall(m)");
    Input(height).equals(4.).help("Wall height (m)");
//    Input(margin).equals(0.15);
    Input(roofPitch).equals(26.).help("Pitch (slope) of roof (degrees)");
    Input(reflection).equals(0.1).help("Outer reflection of greenhouse construction (excl. cover) [0;1]");
    Input(horizontalScreenState).imports("construction/shelter[horizontalScreenState]");

    Output(width).help("Total width of greenhouse across spans (m)");
    Output(groundArea).help("Total area covered by greenhouse (m2)");
    Output(roofArea).help("Total area of greenhouse roof (m2)");
    Output(sideWallsArea).help("Total area of side walls (m2)");
    Output(endWallsArea).help("Total area of end walls (m2)");
    Output(gablesArea).help("Total area of gables (m2)");
    Output(coverArea).help("Total area of greenhouse cover (m2)");
    Output(coverPerGroundArea).help("Area to ground cover ratio (m2/m2)");
    Output(indoorsVolume).help("Total greenhouse volume;"
                               "roof volume include in proportion to horizontal screen state (m3)");
    Output(indoorsAverageHeight).help("Aveage height computed from indoorsVolume/groundArea");
}

void Geometry::reset() {
    double roofHeight = std::tan(roofPitch*PI/180.)*spanWidth/2.,
           roofWidth = std::hypot(roofHeight, spanWidth/2.);
    width = numSpans*spanWidth;
    groundArea = width*length;
    roofArea = 2*numSpans*roofWidth*length;
    sideWallsArea = 2*length*height;
    endWallsArea = 2*width*height;
    gablesArea = numSpans*roofHeight*spanWidth,

    coverArea = sideWallsArea + endWallsArea + gablesArea + roofArea;
    coverPerGroundArea = coverArea/groundArea;

    roofVolume  = length*gablesArea/2.,
    indoorsVolume = groundArea*height + roofVolume;
    indoorsAverageHeight = indoorsVolume/groundArea;
}

void Geometry::update() {
    double roofVolumeIndoors =  roofVolume*(1.-horizontalScreenState);
    indoorsVolume = groundArea*height + roofVolumeIndoors;
    indoorsAverageHeight = indoorsVolume/groundArea;
}

} //namespace

