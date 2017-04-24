/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef MUSSEL_BED_STARFISH_SAL_SCALE_H
#define MUSSEL_BED_STARFISH_SAL_SCALE_H

#include <usbase/model.h>

namespace mussel_bed {

class StarfishSalScale : public UniSim::Model
{
public:
    StarfishSalScale(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double smean;

    // Variables
    double value;
};

} //namespace


#endif

