/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_LEAF_RADIATION_ABSORBED_H
#define VG_LEAF_RADIATION_ABSORBED_H

#include <base/box.h>

namespace vg {

class LeafRadiationAbsorbed : public base::Box
{
public:
    LeafRadiationAbsorbed(QString name, QObject *parent);
    void initialize();
    void update();
private:
    // Inputs
    double
        xGaussLowerside, wGaussLowerside,
        xGaussUpperside, wGaussUpperside,
        kLw, lai, indoorsLight,
        lightAbsorptivity, emissivity,
        growthLightLw, growthLightViewFactor,
        floorTemperature, floorEmissivity,
        leafTemperature, coverTemperature, screensTemperature, screensMaxState,
        shelterOutgoingLwAbsorptivity,
        coverPerGroundArea,
        pipeInflowTemperature;

    // Outputs
    double value, lightAbsorbed, heatingAbsorbed,
        growthLightLwAbsorbed,
        floorLwAbsorbed,
        shelterLoss;

    // Data
    double lwTransmissionLowerside, lwTransmissionUpperside;

    struct PipeInfo {
        const double *density, *diameter, *emissivity;
        double area() {return (*density)*(*diameter)/1000./2; }  // Only half of the area faces upwards
    };
    QVector<PipeInfo> pipeInfos;

    // Methods
    void setLightAbsorbed();
    void setGrowthLightLwAbsorbed();
    void setFloorLwAbsorbed();
    void setShelterLoss();
    void setHeatingAbsorbed();
};

} //namespace

#endif
