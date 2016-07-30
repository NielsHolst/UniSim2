#include "dialog_base.h"

namespace base {

DialogBase* DialogBase::_dialog = 0;

DialogBase& dialog() {
    Q_ASSERT(DialogBase::_dialog);
    return *DialogBase::_dialog;
}

DialogBase::DialogBase(QWidget *parent)
    : QTextEdit(parent),
      _errorCount(0)
{
    _dialog = this;
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

