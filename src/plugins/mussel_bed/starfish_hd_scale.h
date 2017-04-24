/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera (antonio.aguera@gmail.com).
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef MUSSEL_BED_STARFISH_HD_SCALE_H
#define MUSSEL_BED_STARFISH_HD_SCALE_H

#include <usbase/model.h>

namespace mussel_bed {

class StarfishHdScale : public UniSim::Model
{
public:
    StarfishHdScale(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double vmax, mBiomass;

    // Variables
    double scale, value;
};

} //namespace


#endif
