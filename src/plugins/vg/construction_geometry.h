/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CONSTRUCTION_GEOMETRY_H
#define CONSTRUCTION_GEOMETRY_H

#include <base/box.h>

namespace vg {

class ConstructionGeometry : public base::Box
{
public:
    ConstructionGeometry(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double spanWidth, length, height, margin, roofPitch, shade, screensMaxState;
    int numSpans;
    bool hasHorizontalScreens;

    // Output
    double width, groundArea,
        roofArea, sideWallsArea, endWallsArea, gablesArea,
        coverArea, coverPerGroundArea,
        indoorsVolume, indoorsAverageHeight;

    // Data
    double roofVolume;
};

} //namespace


#endif
