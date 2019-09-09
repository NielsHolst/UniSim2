/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QApplication>
#include <QProgressBar>
#include "progress_indicator_progress_bar.h"


namespace base {

ProgressIndicatorProgressBar::ProgressIndicatorProgressBar(QProgressBar *progressBar, QObject *parent)
    : ProgressIndicatorBase(parent),
      _progressBar(progressBar)
{
    _progressBar->setFormat(" %p% of %ms");
}

void ProgressIndicatorProgressBar::show(int current, int total) {
    _progressBar->setMaximum(total);
    _progressBar->setValue(current);
    _progressBar->show();
     qApp->processEvents();
}

} //namespace


