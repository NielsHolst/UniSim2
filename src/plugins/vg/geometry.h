/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <base/box.h>

namespace vg {

class Geometry : public base::Box
{
public:
    Geometry(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double orientation, spanWidth, length, height, roofPitch, reflection, horizontalScreenState;
    int numSpans;

    // Output
    double width, groundArea,
        roofArea, sideWallsArea, endWallsArea, gablesArea,
        coverArea, coverPerGroundArea,
        indoorsVolume, indoorsVolumeTotal, indoorsAverageHeight;

    // Data
    double roofVolume;
};

} //namespace


#endif
