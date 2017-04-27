/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "surface_water_degradation.h"
#include <cmath>
#include <base/publish.h>
#include "general.h"

using namespace base;
using namespace std;

namespace PestTox {

PUBLISH (SurfaceWaterDegradation )
	
SurfaceWaterDegradation ::SurfaceWaterDegradation (QString name, QObject *parent)
	: Box(name, parent)
{

    Input(inflow).equals(0.).help("Runoff from the field (g/ha)");
    Input(inflow2).equals(0.).help("Spray drift off-field edge (g/ha)");
    Input(DT50).equals(4.5).help("Biodegradation half-life (days)");
    Input(Tw).imports("soilTemperature[value]").help("Actual water temperature (°C). Same as soil temperature");
    Input(pondWidth).equals(1.).help("Width of the pond or channel (m)");
    Input(pondHeight).equals(1.).help("Height of the pond or channel (m)");
    Input(pondLength).equals(1.).help("Length of the pond or channel (m)");
    Input(fieldArea).imports("field[area]").help("Area of whole field (m2)");
    Input(RunoffAmount).equals(0.).help("Runoff amount (mm)");

    Output(concentration).help("Concetration of the pesticide in the pond or channel(mg/L)");
    Output(outflow);
    Output(fwdT).help("Temperature correction factor");
    Output(kwd).help("Temperature corrected biodegradation rate constant (per day)");
    Output(pondVolume).help("Volume of the pond or channel (L)");
    Output(fieldWater).help("Volume of water runoff from  the field (L)");
    Output(runoff).help("Pesticide concentration in the runoff: from  the field (mg/L)");
    Output(sprayDrift).help("Pesticide concentration: from spray drift (mg/L)");

}

void SurfaceWaterDegradation ::update() {

    const double Tref = 25.;
    double k = log(2)/DT50;
    fwdT = (Tw > 0) ? pow(Q10,(Tw - Tref)/10.) : 0;
    kwd = k*fwdT;
    fieldWater = (RunoffAmount > 0) ? fieldArea*(RunoffAmount*1e-3)*1000. : 0;
    runoff = (fieldWater > 0) ? inflow*1000.*fieldArea*1e-4/fieldWater : 0;
    pondVolume = pondWidth*pondHeight*pondLength*1000.;
    sprayDrift = inflow2*1000.*fieldArea*1e-4/pondVolume;
    outflow = concentration*kwd;
    concentration += (fieldWater > 0) ? runoff + sprayDrift - concentration: runoff + sprayDrift - outflow;

}

} //namespace

