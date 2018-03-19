/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "transmission_cover.h"

using namespace base;

namespace vg {

PUBLISH(TransmissionCover)

/*! \class TransmissionCover
 * \brief Greenhouse light and energy transmission through cover
 */

TransmissionCover::TransmissionCover(QString name, QObject *parent)
    : TransmissionBase(name, parent)
{
}

bool TransmissionCover::applyScreens() const {
    return false;
}
} //namespace

