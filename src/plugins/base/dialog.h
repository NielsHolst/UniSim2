/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
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
    void loadWithFilePicker();
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
