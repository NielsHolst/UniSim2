/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include "dialog_stub.h"

namespace base {

DialogStub::DialogStub(QObject *parent)
    : DialogBase(parent)
{
}

void DialogStub::progress(int, int) {
}

void DialogStub::finished() {
}

QProgressBar* DialogStub::progressBar() {
    return nullptr;
}

void DialogStub::finishProgress() {
    std::cout << "Finished\n";
}

void DialogStub::information(QString s) {
    std::cout << "DIALOG INFORMATION: " << qPrintable(s) << "\n";
    _information = s;
}

void DialogStub::messageImpl(QString s) {
    std::cout << "DIALOG MESSAGE: " << qPrintable(s) << "\n";
    _message = s;
}

void DialogStub::errorImpl(QString s) {
    std::cout << "DIALOG ERROR: " << qPrintable(s) << "\n";
    _error = s;
}

QString DialogStub::getInformation() const {
    return _information;
}

QString DialogStub::getError() const {
    return _error;
}

}
