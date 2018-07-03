/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMap>
#include <base/data_grid.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/interpolate.h>
#include <base/publish.h>
#include "cover.h"

using namespace base;

namespace vg {

PUBLISH(Cover)

Cover::Cover(QString name, QObject *parent)
    : SurfaceRadiationOutputs(name, parent)
{
    help("computes cover light and heat characteristics");
    Input(greenhouseReflection).imports("geometry[reflection]");
    Input(chalk).imports("controllers/chalk[value]");
    Input(latitude).imports("calendar[latitude]");
    Input(azimuth).imports("calendar[azimuth]");
    Input(area).imports("..[area]");
    Input(windSpeed).imports("outdoors[windSpeed]");
    Input(directTransmissionFile).equals("input/direct_transmission_single.txt")
            .help("Table of direct light transmittance depending on latitude and sun azimuth");
    Input(emissivity).equals(0.84).help("Emissivity of long-wave radiation [0;1]");
    Input(absorptivity).equals(0.04).help("Absorptivity of long-wave radiation [0;1]");
    Input(transmissivity).equals(0.83).help("Transmissivity to short-wave radiation [0;1]");
    Input(haze).equals(0.).help("Proportion of direct light transmitted as diffuse light [0;1]");
    Input(U4).equals(7.5).help("Heat transfer coefficient at a wind speed of 4 m/s (W/m2/K)");
    Input(specificHeatCapacity).equals(700.).help("Area-specific heat capacity (J/m2 cover/K)");

    Output(U).help("Heat transfer coefficient, corrected for wind speed (W/m2/K)");
    Output(heatCapacity).help("Whole-cover heat capacity (J/K)");
}

void Cover::initialize() {
    QString filePath = environment().inputFileNamePath(directTransmissionFile);
    dirTransTable = new DataGrid(filePath, this);
}

void Cover::reset() {
    update();
//    resetRadiationOutputs();
//    U = U4;
}

void Cover::update() {
    double directLightfactor = interpolate(*dirTransTable, latitude, azimuth),
           tr = (1-greenhouseReflection)*(1-chalk);
    set( SurfaceRadiation().asCover(tr*transmissivity, tr*transmissivity*directLightfactor,
                                    absorptivity, emissivity) );
    double k = (windSpeed <= 4) ? (2.8 + 1.2*windSpeed)/7.6 : pow(windSpeed,0.8)/pow(4.,0.8);
    U = k*U4;
    heatCapacity = specificHeatCapacity*area;
}

} //namespace

