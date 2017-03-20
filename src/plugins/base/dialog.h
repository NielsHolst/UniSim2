#ifndef BASE_DIALOG_H
#define BASE_DIALOG_H

#include "dialog_base.h"

namespace base {

class DialogWidget;

class Dialog : public DialogBase
{
public:
    Dialog(QObject *parent);
    void progress(int current, int total);
    void finished();
    void message(QString s);
    void information(QString s);
    void errorImpl(QString s);
    QTextEdit* textEdit();
    void setFont(QString family, int pt);
    QFont font();
private:
    DialogWidget *_widget;
};

}

#endif
