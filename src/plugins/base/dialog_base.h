#ifndef DIALOG_BASE_H
#define DIALOG_BASE_H

#include <QTextEdit>
#include <QString>

namespace base {

class DialogBase : public QTextEdit
{
public:
    DialogBase(QWidget *parent);
    virtual void progress(int current, int total) = 0;
    virtual void finished() = 0;
    virtual void message(QString s) = 0;
    virtual void information(QString s) = 0;
    void error(QString s);
    friend DialogBase& dialog();
    void resetErrorCount();
    int errorCount() const;
private:
    // singleton data
    static DialogBase *_dialog;
    // data
    int _errorCount;
    // methods
    virtual void errorImpl(QString s) = 0;
};

// singleton method
DialogBase& dialog();

}

#endif
