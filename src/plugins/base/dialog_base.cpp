#include "dialog_base.h"

namespace base {

DialogBase* DialogBase::_dialog = 0;

DialogBase& dialog() {
    Q_ASSERT(DialogBase::_dialog);
    return *DialogBase::_dialog;
}


DialogBase::DialogBase(QWidget *parent)
    : QTextEdit(parent)
{
    _dialog = this;
}

}

