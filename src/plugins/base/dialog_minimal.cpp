/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QProgressBar>
#include <QProgressDialog>
#include "dialog_minimal.h"

namespace base {

DialogMinimal::DialogMinimal(QObject *parent)
    : DialogQuiet(parent)
{
    _progressDialog = new QProgressDialog;
    _progressDialog->setWindowModality(Qt::WindowModal);
    _progressDialog->setLabel(new QLabel("Running...", _progressDialog));
    _progressDialog->setBar(_progressBar = new QProgressBar);

    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->availableGeometry();
    int W = screen.width();
    QSize size = _progressDialog->size();
    _progressDialog->resize(W/4, size.height());

    _progressDialog->show();
}

QProgressBar* DialogMinimal::progressBar() {
    Q_ASSERT(_progressBar);
    return _progressBar;
}

void DialogMinimal::finishProgress() {
    _progressDialog->close();
}

}
