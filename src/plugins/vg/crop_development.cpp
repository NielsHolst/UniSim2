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
    Input(duration).equals(1).help("Duration in physiological time units").unit("ptu");
    Input(step).equals(0.05).help("Development step per day").unit("ptu/d");
    Input(timeStepDays).imports("calendar[timeStepDays]").unit("d");
    Output(rate).help("Development rate (per day)").unit("/d");
}

void CropDevelopment::reset() {
    update();
}

void CropDevelopment::update() {
    if (duration == 0.)
        ThrowException("Duration must be > 0").value(duration).context(this);
    rate = step/duration*timeStepDays;
}

} //namespace

