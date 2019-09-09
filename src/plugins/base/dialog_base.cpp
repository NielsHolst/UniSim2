/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QApplication>
#include <QProgressBar>
#include <QTextEdit>
#include "dialog_base.h"
#include "exception.h"

namespace base {

DialogBase* DialogBase::_dialog = nullptr;
QStack<QString> DialogBase::_messages;

DialogBase& dialog() {
    Q_ASSERT(DialogBase::_dialog);
    return *DialogBase::_dialog;
}

DialogBase::DialogBase(QObject *parent)
    : QObject(parent),
      _errorCount(0)
{
    _dialog = this;
    resetProgress();
}

void DialogBase::resetProgress() {
    _nextShowProgress = 0.01;
}

void DialogBase::updateProgress(const ProgressInfo &info) {
    double progress = double(info.step + (info.iteration-1)*info.steps)/info.steps/info.iterations;
    if (progress > _nextShowProgress) {
        QProgressBar *bar = progressBar();
        if (bar) {
            bar->setFormat(" %p% of %ms");
            double total = info.time.elapsed()/progress;
            bar->setMaximum(static_cast<int>(total)/1000);
            bar->setValue(static_cast<int>(info.time.elapsed())/1000);
            bar->show();
            qApp->processEvents();
            _nextShowProgress += 0.01;
        }
    }
}

QTextEdit* DialogBase::textEdit() {
    ThrowException("Dialog has no TextEdit widget");
}

void DialogBase::message(QString s) {
    _message = s;
    messageImpl(s);
}

void DialogBase::pushMessage() {
    _messages.push(_message);
}

void DialogBase::popMessage() {
    if (_messages.isEmpty())
        ThrowException("Unexpected empty message stack").context(this);
    message(_messages.pop());
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

