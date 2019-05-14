/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_PIPE_H
#define VG_PIPE_H

#include <QMap>
#include <base/box.h>

namespace vg {

class Pipe : public base::Box
{
public:
    Pipe(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    QString material;
    double density, diameter, flowRate, minTemperature, maxTemperature;
    // Outputs
    double a, b, emissivity;
    // Methods
    void parseMaterial();
    // Data
    struct MaterialInputs {
        double a, b, emissivity;
    };
    QMap<QString, MaterialInputs> materialInputs;
};

} //namespace


#endif
