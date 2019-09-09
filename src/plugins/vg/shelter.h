/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SHELTER_H
#define SHELTER_H

#include "shelter_base.h"

namespace base {
class Box;
class BoxBuilder;
}

namespace vg {

struct SurfaceRadiation;

class Shelter : public ShelterBase
{
public:
    Shelter(QString name, QObject *parent);
    void amend();
    void initialize();
    void update();

private:
    // Inputs
    double groundArea;
    // Outputs
    double heatCapacityCoversPerGround, heatCapacityScreensPerGround,
        screensEffectiveArea, screensPerGroundArea,
        screensMaxState, horizontalScreenState;
//        ventsEffectiveArea, ventsMaxEffectiveArea;
    bool hasHorizontalScreen;
    // Data
    struct FaceInfo {
        const SurfaceRadiation *sr;
        const double *diffuseLightTransmitted, *directLightTransmitted, *totalLightTransmitted,
            *lightAbsorbedCover, *lightAbsorbedScreens,
            *haze, *U,
            *airTransmissivity,
            *area, *relativeArea, *screensEffectiveArea,
            *heatCapacityCover, *heatCapacityScreens,
            *screensMaxState;
    };
    QVector<Box*> _horizontalScreens;

    QVector<FaceInfo> infos;
    // Methods
    void amendShelter(base::BoxBuilder &builder, QString shelterName);
    void initHorizontalScreens();
};
} //namespace


#endif
