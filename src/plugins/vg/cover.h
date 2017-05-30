/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_COVER_H
#define VG_COVER_H

#include <base/box.h>
#include "surface_radiation_outputs.h"

namespace base {
    class DataGrid;
}

namespace vg {

class Cover : public vg::SurfaceRadiationOutputs
{
public:
    Cover(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    QString directTransmissionFile;
    double
        greenhouseReflection, chalk,
        latitude, azimuth, area,
        U4, windSpeed, emissivity, absorptivity, transmissivity,
        specificHeatCapacity, haze;

    // Outputs
    double U, heatCapacity;

    // Data
    base::DataGrid *dirTransTable;
};
} //namespace


#endif
