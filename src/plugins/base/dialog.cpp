#include <QMainWindow>
#include "dialog.h"
#include "exception.h"
#include "dialog_widget.h"

namespace base {

Dialog::Dialog(QObject *parent)
    : DialogBase(parent)
{
    setObjectName("dialog");
    QMainWindow *wParent = dynamic_cast<QMainWindow*>(parent);
    if (!wParent)
        ThrowException("Parent must be a QWidget").context(this);
    _widget = new DialogWidget(wParent);
    _widget->init();
}

QTextEdit* Dialog::textEdit() {
    return _widget;
}

void Dialog::setFont(QString family, int pt) {
    _widget->setFont(family, pt);
}

QFont Dialog::font() {
    return _widget->font();
}

void Dialog::progress(int current, int total) {
    _widget->progress(current, total);
}

void Dialog::finished() {
    _widget->finished();
}

void Dialog::message(QString s) {
    _widget->message(s);
}

void Dialog::information(QString s) {
    _widget->information(s);
}

void Dialog::errorImpl(QString s) {
    _widget->errorImpl(s);
}

}
