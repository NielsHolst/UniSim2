/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMap>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "interpolate.h"
#include "cover.h"
#include "data_grid.h"

using std::min;
using namespace base;

namespace vg {

PUBLISH(Cover)


/*! \class Cover
 * \brief Characterises the light and energy transmission of a cover part
 *
 * A greenhouse has seven surfaces, each represented by one ConstructionCover object. The name of the object
 * identifies which surface it represents: "Roof1", "Roof2", "Side1", "Side2", "End1", "End2", "Floor".
 *
 * Inputs
 * ------
 * - _greenhouseShade_ is the fraction of light caught by the greenhouse construction [0;1]
 * - _chalk_ is the chalk efficacy [0;1]
 * - _directTransmissionFile_ is the name of a file with coefficients correcting the transmissivity for direct light,
 * according to latitude (rows) and sun azimuth (columns)
 * - _latitude_ is geographical latitude of the greenhouse [-180;180]
 * - _azimuth_ is the azimuth of the sun relative to north [-180;180]
 * - _area_ is the area of the cover [m<SUP>2</SUP>]
 * - _windSpeed_ is the outdoors wind speed [m/s]
 * - _U4_ is the heat transfer coefficient of the material at a windspeed of 4 m/s [W/m<SUP>2</SUP>/K]
 * - _emissivity_ is the effectiveness in emitting energy as thermal radiation [0;1].
 * - _absorptivity_ is the proportion of light absorbed [0;1]
 * - _transmissivity_ is the proportion of diffuse light transmitted through the material [0;1]
 * - _haze_ is the proportion of direct light becoming dispersed on passage through the material [0;1]
 * - _specificHeatCapacity_ is the area-specific heat capacity [J/m<SUP>2</SUP> cover/K]

 * Output
 * ------
 * - _U_ is the wind-corrected heat transfer coefficient of the material [W/m<SUP>2</SUP>/K]
 * - _heatCapacity_ is the heat capacity of the whole cover [J/K]
 */

Cover::Cover(QString name, QObject *parent)
    : SurfaceRadiationOutputs(name, parent)
{
    Input(greenhouseShade).imports("geometry[shade]");
    Input(chalk).imports("controllers/chalk[signal]");
    Input(directTransmissionFile).imports("direct_transmission_single.txt");
    Input(latitude).imports("calendar[latitude]");
    Input(azimuth).imports("calendar[azimuth]");
    Input(area).imports("..[area]");
    Input(windspeed).imports("outdoors[windspeed]");
    Input(U4).equals(7.5);
    Input(emissivity).equals(0.84);
    Input(absorptivity).equals(0.04);
    Input(transmissivity).equals(1.);
    Input(haze).equals(0.);
    Input(antiReflection).equals(false);
    Input(specificHeatCapacity).equals(840.);

    Output(U);
    Output(heatCapacity);
}

void Cover::initialize() {
    QDir inputFolder = environment().state.dir.input;
    dirTransTable = new DataGrid(inputFolder.absoluteFilePath(directTransmissionFile), this);
}

void Cover::reset() {
    resetRadiationOutputs();
    U = U4;
}

void Cover::update() {
    double directLightfactor = interpolate(*dirTransTable, latitude, azimuth),
           tr = (1-greenhouseShade)*(1-chalk);
    set( SurfaceRadiation().asCover(tr*transmissivity, tr*transmissivity*directLightfactor,
                                    absorptivity, emissivity) );
    double k = (windspeed <= 4) ? (2.8 + 1.2*windspeed)/7.6 : pow(windspeed,0.8)/pow(4.,0.8);
    U = k*U4;
    heatCapacity = specificHeatCapacity*area;
}

} //namespace

