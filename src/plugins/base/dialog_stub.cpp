#include "dialog_stub.h"

namespace base {

DialogStub::DialogStub(QWidget *parent)
    : DialogBase(parent)
{
}

void DialogStub::information(QString s) {
    _information = s;
}

void DialogStub::error(QString s) {
    _error = s;
}

QString DialogStub::getInformation() const {
    return _information;
}

QString DialogStub::getError() const {
    return _error;
}

}
