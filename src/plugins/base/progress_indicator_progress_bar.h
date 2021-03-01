/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PROGRESS_INDICATOR_PROGRESS_BAR_H
#define BASE_PROGRESS_INDICATOR_PROGRESS_BAR_H

#include "progress_indicator_base.h"
#include <QObject>
#include <QTime>

class QProgressBar;

namespace base {

class ProgressIndicatorProgressBar : public ProgressIndicatorBase
{
public:
    ProgressIndicatorProgressBar(QProgressBar *progressBar, QObject *parent);
    void show(int current, int total);
private:
    // Data
    QProgressBar *_progressBar;
    double _nextShowProgress;
};


} //namespace

#endif

