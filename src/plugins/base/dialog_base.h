#ifndef DIALOG_BASE_H
#define DIALOG_BASE_H

#include <QTextEdit>
#include <QString>

namespace base {

class DialogBase : public QTextEdit
{
public:
    DialogBase(QWidget *parent) : QTextEdit(parent) {}
    virtual void information(QString s) = 0;
    virtual void error(QString s) = 0;
};

}

#endif
