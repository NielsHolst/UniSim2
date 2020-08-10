/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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
    double orientation, spanWidth, length, height, roofPitch, reflection;
    int numSpans;
    QVector<double> airSpaces;
    // Output
    double width, groundArea,
        roofHeight, roofArea, sideWallsArea, endWallsArea, gablesArea,
        coverArea, coverPerGroundArea,
        indoorsVolumeTotal, averageHeight,
        screenedVolume, roomVolume,
        screenedVolumeChange, roomVolumeChange;
    // Data
    double _roofVolume, _prevRoomVolume;
};

} //namespace


#endif
