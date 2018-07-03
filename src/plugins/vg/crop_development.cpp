/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "crop_development.h"

using namespace base;

namespace vg {

PUBLISH(CropDevelopment)

CropDevelopment::CropDevelopment(QString name, QObject *parent)
	: Box(name, parent)
{
    help("models crop development");
    Input(physDuration).equals(200).help("Duration in physiological time units (ptu)");
    Input(physStep).equals(10).help("Development step (ptu/day)");
    Output(rate).help("Development rate (per day)");
    Output(duration).help("Development duration (days)");
}

void CropDevelopment::reset() {
    if (physDuration < 0.)
        ThrowException("physDuration must be positive").value(physDuration).context(this);
}

void CropDevelopment::update() {
    rate = physStep/physDuration;
    duration = (rate > 0.) ? 1./rate : 365.;
    if (duration > 365.)
        duration = 365.;
}

} //namespace

