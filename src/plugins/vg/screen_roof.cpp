/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "screen_roof.h"

using namespace base;

namespace vg {

PUBLISH(ScreenRoof)

ScreenRoof::ScreenRoof(QString name, QObject *parent)
    : Screen(name, parent)
{
    help("models a screen towards the roof");
    Input(groundArea).imports("construction/geometry[groundArea]",CA);
    Input(roofHeight).imports("construction/geometry[roofHeight]",CA);
}

double ScreenRoof::computeMaxArea() {
    // A roof screen covers half of the ground area;
    // we assume a horizontal roof screen
    return groundArea/2.;
}

double ScreenRoof::computeDepth() {
    // First air space below the roof always uses half of the roof height for its depth
    // (thus assuming a horizontal roof screen)
    return roofHeight/2.;
}


} //namespace

