/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SHELTER_FACE_H
#define SHELTER_FACE_H

#include "shelter_base.h"

namespace vg {

class ShelterFace : public ShelterBase
{
public:
    ShelterFace(QString name, QObject *parent);
    void amend();
    void initialize();
    void reset();
    void update();

private:
    // Inputs
    double
        roofArea, sideWallsArea, endWallsArea, gablesArea, groundArea,
        outdoorsDirectRadiation, outdoorsDiffuseRadiation;

    // Outputs
    double area, relativeArea, areaPerGround, screensMaxState;

    // Data
    bool _hasScreens;
    const double
        *pCoverU, *pCoverHaze,
        *pScreensU,
        *pScreensHaze,
        *pScreensAirTransmission;

    struct Light {
        struct {
            const double *tra, *abs;
        } diffuse, direct;
        void fetch(base::Box *model);
    };
    Light face, cover, screens;

    const SurfaceRadiation *pCoverSurfaceRadiation, *pScreensSurfaceRadiation;
    QVector<Box*> _screens;

    // Methods
    void updateU();
    void updateHaze();
    void updateAirTransmission();
    void updateLightTransmission();

};
} //namespace


#endif
