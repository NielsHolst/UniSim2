/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PIPE_FORCED_H
#define PIPE_FORCED_H

#include <base/box.h>

namespace vg {

class PipeForced : public base::Box
{
public:
    PipeForced(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double area, Tair, innerDiameter, waterVolume, flowRate, k, b, Tinflow,
        knownToutflow;
    // Outputs
    double transitTime, Toutflow, effect;
    // Methods
    void updateToutflow();
    void updateTransitTime();
    void updateEffect();
};

} //namespace


#endif