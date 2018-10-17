#ifndef BASE_DIALOG_MINIMAL_H
#define BASE_DIALOG_MINIMAL_H

#include "dialog_quiet.h"

class QProgressBar;
class QProgressDialog;

namespace base {

class DialogMinimal : public DialogQuiet
{
public:
    DialogMinimal(QObject *parent);
    QProgressBar* progressBar();
    void finishProgress();
private:
    QProgressBar *_progressBar;
    QProgressDialog *_progressDialog;
};

}

#endif
