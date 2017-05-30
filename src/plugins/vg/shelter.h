/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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
