/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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
    : SurfaceRadiationOutputs(name, parent), _isInitialized(false)
{
    help("computes cover light and heat characteristics");
    Input(greenhouseReflection).imports("geometry[reflection]").unit("[0;1]");
    Input(chalk).imports("controllers[chalk]").unit("[0;1]");
    Input(latitude).imports("calendar[latitude]").unit("[0;90]");
    Input(azimuth).imports("calendar[azimuth]").unit("[-180;180]");
    Input(area).imports("..[area]").unit("m2");
    Input(windSpeed).imports("outdoors[windSpeed]").unit("m/s");
    Input(directTransmissionFile).equals(":/data/vg/direct_transmission_single.txt").unit("String")
            .help("Table of direct light transmittance depending on latitude and sun azimuth");
    Input(emissivity).equals(0.84).help("Emissivity of long-wave radiation").unit("[0;1]");
    Input(absorptivity).equals(0.04).help("Absorptivity of long-wave radiation").unit("[0;1]");
    Input(transmissivity).equals(0.83).help("Transmissivity to short-wave radiation").unit("[0;1]");
    Input(haze).equals(0.).help("Proportion of direct light transmitted as diffuse light").unit("[0;1]");
    Input(U4).equals(7.5).help("Heat transfer coefficient at a wind speed of 4 m/s").unit("W/m2/K");
    Input(specificHeatCapacity).equals(700.).help("Area-specific heat capacity").unit("J/m2/K");

    Output(U).help("Heat transfer coefficient, corrected for wind speed").unit("W/m2/K");
    Output(heatCapacity).help("Whole-cover heat capacity").unit("J/K");
}

void Cover::initialize() {
    _isInitialized = true;
    bool isInResources = (directTransmissionFile.at(0) == ":");
    QString filePath = isInResources ? directTransmissionFile : environment().inputFileNamePath(directTransmissionFile);
    dirTransTable = new DataGrid(filePath, this);
}

void Cover::reset() {
    if (!_isInitialized)
        ThrowException("Program error: Not initialized before reset").context(this);
    update();
//    resetRadiationOutputs();
//    U = U4;
}

void Cover::update() {
    double directLightfactor = interpolate(*dirTransTable, latitude, azimuth),
           tr = (1-greenhouseReflection)*(1-chalk);
    // Override direct reflectance correction; it is not well estimated
    directLightfactor = 1.;
    set( SurfaceRadiation().asCover(tr*transmissivity, tr*transmissivity*directLightfactor,
                                    absorptivity, emissivity) );
    double k = (windSpeed <= 4) ? (2.8 + 1.2*windSpeed)/7.6 : pow(windSpeed,0.8)/pow(4.,0.8);
    U = k*U4;
    heatCapacity = specificHeatCapacity*area;
}

} //namespace

