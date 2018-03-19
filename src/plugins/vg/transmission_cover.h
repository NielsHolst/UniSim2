/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_TRANSMISSION_COVER_H
#define VG_TRANSMISSION_COVER_H

#include "transmission_base.h"

namespace vg {

class TransmissionCover : public TransmissionBase
{
public:
    TransmissionCover(QString name, QObject *parent);
    bool applyScreens() const;
};
} //namespace


#endif
