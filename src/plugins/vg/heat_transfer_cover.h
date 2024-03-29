/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HEAT_TRANSFER_COVER_H
#define HEAT_TRANSFER_COVER_H
#include "heat_transfer_shelter.h"

namespace vg {

class HeatTransferCover : public HeatTransferShelter
{
public:
    HeatTransferCover(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double skyTemperature, outdoorsTemperature;
};

} //namespace


#endif
