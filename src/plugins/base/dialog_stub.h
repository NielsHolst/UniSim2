#ifndef BASE_DIALOG_STUB_H
#define BASE_DIALOG_STUB_H

#include "dialog_base.h"

namespace base {

class DialogStub : public DialogBase
{
public:
    DialogStub(QObject *parent);
    void progress(int current, int total);
    void finished();
    QProgressBar* progressBar();
    void finishProgress();
    void information(QString s);
    QString getInformation() const;
    QString getError() const;
private:
    QString _message, _information, _error;
    void messageImpl(QString s);
    void errorImpl(QString s);
};

}

#endif
