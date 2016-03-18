#ifndef DIALOG_BASE_H
#define DIALOG_BASE_H

#include <QTextEdit>
#include <QString>

namespace base {

class DialogBase : public QTextEdit
{
public:
    DialogBase(QWidget *parent);
    virtual void message(QString s) = 0;
    virtual void information(QString s) = 0;
    virtual void error(QString s) = 0;
    friend DialogBase& dialog();
private:
    static DialogBase *_dialog;
};

DialogBase& dialog();

}

#endif
