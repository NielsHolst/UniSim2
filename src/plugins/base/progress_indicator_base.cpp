/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "progress_indicator_base.h"


namespace base {

ProgressIndicatorBase::ProgressIndicatorBase(QObject *parent)
    : QObject(parent)
{
    setObjectName("ProgressIndicator");
    reset();
}

void ProgressIndicatorBase::reset() {
    _nextShowProgress = 0;
}

void ProgressIndicatorBase::update(const UpdateInfo &info) {
    double progress = double(info.step + (info.iteration-1)*info.steps)/info.steps/info.iterations;
    if (progress > _nextShowProgress) {
        double total = info.time.elapsed()/progress;
        show(static_cast<int>(info.time.elapsed())/1000, static_cast<int>(total)/1000);
        _nextShowProgress += 0.01;
    }
}

} //namespace


