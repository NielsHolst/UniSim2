#include<QTextEdit>
#include "dialog_base.h"
#include "exception.h"

namespace base {

DialogBase* DialogBase::_dialog = 0;

DialogBase& dialog() {
    Q_ASSERT(DialogBase::_dialog);
    return *DialogBase::_dialog;
}

DialogBase::DialogBase(QObject *parent)
    : QObject(parent),
      _errorCount(0)
{
    _dialog = this;
}

QTextEdit* DialogBase::textEdit() {
    ThrowException("Dialog has no TextEdit widget");
    return 0;
}

void DialogBase::error(QString s) {
    ++_errorCount;
    errorImpl(s);
}

void DialogBase::resetErrorCount() {
    _errorCount = 0;
}

int DialogBase::errorCount() const {
   return _errorCount;
}

}

