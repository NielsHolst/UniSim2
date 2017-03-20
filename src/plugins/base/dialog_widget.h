#ifndef BASE_DIALOG_WIDGET_H
#define BASE_DIALOG_WIDGET_H

#include <QTextCursor>
#include <QTextEdit>
#include "dialog_base.h"
#include "history.h"

class QMainWindow;
class QProgressBar;
class QTextDocument;

namespace base {

class DialogWidget : public QTextEdit
{
    Q_OBJECT
public:
    DialogWidget(QMainWindow *parent);
    ~DialogWidget();
    void init();
    void progress(int current, int total);
    void finished();
    void message(QString s);
    void information(QString s);
    void errorImpl(QString s);
    void setFont(QString family, int pt);
    QFont font();
protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private slots:
    void receivedFocus(QWidget *old, QWidget *now);
private:
    // Data
    bool _init;
    History _history;
    QTextDocument *_textDocument;
    QProgressBar *_progressBar;
    QString _prompt;
    QColor _informationColor, _errorColor;
    bool _firstInformation, _gotoEnd;
    // Methods
    QMainWindow* mainWindow();
    bool resetFont();
    bool fontExists(QString family);
    QString preferredFamily();
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
    void handleCtrlKey(QKeyEvent *event);
    void handleNormalKey(QKeyEvent *event);
};

}

#endif
