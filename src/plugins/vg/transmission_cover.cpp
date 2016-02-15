/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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

