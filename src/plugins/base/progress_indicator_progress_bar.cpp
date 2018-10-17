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


