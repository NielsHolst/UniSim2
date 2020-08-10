/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "screens.h"

using namespace base;

namespace vg {

PUBLISH(Screens)

Screens::Screens(QString name, QObject *parent)
    : Box(name, parent)
{
    help("summarizes attributes of Screen boxes");
    Input(states).imports("./*[state]");
    Input(depths).imports("./*[depth]");
    Input(maxAreas).imports("./*[maxArea]");
    Input(Utops).imports("./*[Utop]");
    Input(Ubottoms).imports("./*[Ubottom]");
    Input(heatCapacities).imports("./*[heatCapacity]");
    Output(Utop).help("Joint Utop of all screens").unit("W/m2/K");
    Output(Ubottom).help("Joint Ubottom of all screens").unit("W/m2/K");
    Output(screenedVolume).help("Total volume screened between screens and cover").unit("m2");
    Output(heatCapacity).help("Total heat capacity of drawn sheets").unit("J/K");
}

void Screens::amend() {
    BoxBuilder builder(this);
    builder.
    box("ScreensAirTransmissivity").name("airTransmissivity").
    endbox();
}

void Screens::reset() {
    update();
}

void Screens::update() {
    // Count screens
    int n = states.size();
    if (n==0) {
        Utop = Ubottom = phys_math::infinity();
        screenedVolume = 0.;
        heatCapacity = 0.;
        return;
    }

    // Update joint U values
    double Rtop = 0.,
           Rbottom = 0.;
    for (int i=0; i<n; ++i) {
        Rtop    += states.at(i)/Utops.at(i);
        Rbottom += states.at(i)/Ubottoms.at(i);
    }
    Utop = 1./Rtop;
    Ubottom = 1./Rbottom;

    // Update screened volume using the screen with max state as boundary
    int m = vector_op::whichMax(states);
    double depth = 0.;
    for (int i=0; i<=m; ++i)
        depth += depths.at(i);
    screenedVolume = depth*maxAreas.at(0)*states.at(m); // All maxAreas are the same

    // Update total heat capacity
    heatCapacity = vector_op::sum(heatCapacities);
}

} //namespace

