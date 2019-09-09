/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
    help("defines the greenhouse geometry");
    Input(orientation).equals(90.).help("Compass direction of greenhouse long axis").unit("[0;360]");
    Input(numSpans).equals(1).help("Number of spans").unit("-");;
    Input(spanWidth).equals(40.).help("Width of a span").unit("m");
    Input(length).equals(100.).help("Length of side wall").unit("m");
    Input(height).equals(2.5).help("Wall height").unit("m");
    Input(roofPitch).equals(26.).help("Pitch (slope) of roof").unit("[0;180]");
    Input(reflection).equals(0.1).help("Outer reflection of greenhouse construction (excl. cover").unit("[0;1]");
    Input(horizontalScreenState).imports("construction/shelter[horizontalScreenState]").unit("[0;1]");

    Output(width).help("Total width of greenhouse across spans").unit("m");
    Output(groundArea).help("Total area covered by greenhouse").unit("m2");
    Output(roofArea).help("Total area of greenhouse roof").unit("m2");
    Output(sideWallsArea).help("Total area of side walls").unit("m2");
    Output(endWallsArea).help("Total area of end walls").unit("m2");
    Output(gablesArea).help("Total area of gables").unit("m2");
    Output(coverArea).help("Total area of greenhouse cover").unit("m2");
    Output(coverPerGroundArea).help("Area to ground cover ratio").unit("m2/m2");
    Output(indoorsVolumeTotal).help("Total greenhouse volume").unit("m3");
    Output(indoorsVolume).help("Total greenhouse volume;"
                               "roof volume included in proportion to horizontal screen state").unit("m3");
    Output(indoorsAverageHeight).help("Average height computed from indoorsVolume/groundArea").unit("m");
}

void Geometry::reset() {
    double roofHeight = std::tan(roofPitch*PI/180.)*spanWidth/2.,
           roofWidth = std::hypot(roofHeight, spanWidth/2.);
    width = numSpans*spanWidth;
    groundArea = width*length;
    roofArea = 2*numSpans*roofWidth*length;
    sideWallsArea = 2*length*height;
    endWallsArea = 2*width*height;
    gablesArea = numSpans*roofHeight*spanWidth;

    coverArea = sideWallsArea + endWallsArea + gablesArea + roofArea;
    coverPerGroundArea = coverArea/groundArea;

    roofVolume  = length*gablesArea/2.;
    indoorsVolumeTotal =
    indoorsVolume = groundArea*height + roofVolume;
    indoorsAverageHeight = indoorsVolume/groundArea;
}

void Geometry::update() {
    double roofVolumeIndoors =  roofVolume*(1.-horizontalScreenState);
    indoorsVolume = groundArea*height + roofVolumeIndoors;
    indoorsAverageHeight = indoorsVolume/groundArea;
}

} //namespace

