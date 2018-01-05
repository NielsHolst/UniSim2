/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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