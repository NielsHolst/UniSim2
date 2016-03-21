#ifndef DIALOG_STUB_H
#define DIALOG_STUB_H

#include "dialog_base.h"

namespace base {

class DialogStub : public DialogBase
{
public:
    DialogStub(QWidget *parent);
    void progress(int current, int total);
    void finished();
    void message(QString s);
    void information(QString s);
    void error(QString s);
    QString getInformation() const;
    QString getError() const;
private:
    QString _message, _information, _error;
};

}

#endif
