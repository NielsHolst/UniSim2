#ifndef BASE_DIALOG_H
#define BASE_DIALOG_H

#include <QTextCursor>
//#include <QWidget>
#include "dialog_base.h"
#include "history.h"

class QMainWindow;
class QProgressBar;
class QTextDocument;

namespace base {

class Dialog : public DialogBase
{
    Q_OBJECT
public:
    Dialog(QWidget *parent);
    ~Dialog();
    void progress(int current, int total);
    void finished();
    void message(QString s);
    void information(QString s);
    void errorImpl(QString s);
protected:
//    bool eventFilter(QObject *obj, QEvent *event);
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private slots:
    void receivedFocus(QWidget *old, QWidget *now);
private:
    // Data
    History _history;
    QTextDocument *_textDocument;
    QProgressBar *_progressBar;
    QString _prompt;
    QColor _informationColor, _errorColor;
    bool _firstInformation, _gotoEnd;
    // Methods
    QMainWindow* mainWindow();
    void saveFont();
    void restoreFont();
    QTextCursor getCursor();
    void writePrompt();
    void writeWelcome();
    void insertText(QString text, QColor color = QColor("black"));
    int numLines();
    int linePosition(int i);
    int cursorPosition();
    int cursorLine();
    bool bounceCursor(bool sticky);
    void clearLine();
    void submitCommand();
};

}

#endif
