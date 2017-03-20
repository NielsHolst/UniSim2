#include "dialog_quiet.h"

namespace base {

DialogQuiet::DialogQuiet(QObject *parent)
    : DialogBase(parent)
{
}

void DialogQuiet::progress(int, int) {
}

void DialogQuiet::finished() {
}

void DialogQuiet::message(QString) {
}

void DialogQuiet::information(QString s) {
    _information = s;
}

void DialogQuiet::errorImpl(QString s) {
    _error = s;
}

QString DialogQuiet::getInformation() const {
    return _information;
}

QString DialogQuiet::getError() const {
    return _error;
}

}
