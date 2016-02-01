#ifndef DIALOG_H
#define DIALOG_H

#include <QTextCursor>
#include "dialog_base.h"
#include "history.h"

class QMainWindow;
class QTextDocument;

namespace base {

class Dialog : public DialogBase
{
    Q_OBJECT
public:
    Dialog(QWidget *parent);
    ~Dialog();
    void information(QString s);
    void error(QString s);
protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    // Data
    History _history;
    QTextDocument *_textDocument;
    QString _prompt;
    QColor _informationColor, _errorColor;
    // Methods
    QMainWindow* mainWindow();
    void saveFont();
    void restoreFont();
    QTextCursor getCursor();
    void writePrompt();
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
