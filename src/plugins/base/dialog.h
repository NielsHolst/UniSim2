#ifndef BASE_DIALOG_H
#define BASE_DIALOG_H

#include "dialog_base.h"

namespace base {

class DialogWidget;

class Dialog : public DialogBase
{
public:
    Dialog(QObject *parent);
    QProgressBar* progressBar();
    void finishProgress();
    void information(QString s);
    QTextEdit* textEdit();
    void setFont(QString family, int pt);
    QFont font();
    void writePrompt();
private:
    DialogWidget *_widget;
    QString _message;
    void errorImpl(QString s);
    void messageImpl(QString s);
};

}

#endif
