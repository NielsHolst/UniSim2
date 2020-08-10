/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_DIALOG_BASE_H
#define BASE_DIALOG_BASE_H

#include <QFont>
#include <QObject>
#include <QStack>
#include <QString>
#include <QTime>

class QProgressBar;
class QTextEdit;

namespace base {

class DialogBase : public QObject
{
public:
    DialogBase(QObject *parent);

    struct ProgressInfo{
        QTime time;
        int step, steps, iteration, iterations;
    };
    void resetProgress();
    void updateProgress(const ProgressInfo &info);

    virtual QProgressBar* progressBar() = 0;
    virtual void finishProgress() = 0;
    virtual void information(QString s) = 0;
    virtual QTextEdit* textEdit();
    virtual void loadWithFilePicker();

    virtual void restoreFont() {}
    virtual void setFont(QString, int) {}
    virtual QFont font() { return QFont(); }

    void message(QString s);
    void pushMessage();
    void popMessage();

    void error(QString s);
    friend DialogBase& dialog();
    void resetErrorCount();
    int errorCount() const;

    virtual void writePrompt() {}

private:
    // singleton data
    static DialogBase *_dialog;
    static QStack<QString> _messages;
    // data
    int _errorCount;
    double _nextShowProgress;
    QString _message;
    // methods
    virtual void errorImpl(QString s) = 0;
    virtual void messageImpl(QString s) = 0;
};

// singleton method
DialogBase& dialog();

}

#endif
