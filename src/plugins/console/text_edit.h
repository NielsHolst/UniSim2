#ifndef TEXT_EDIT_H
#define TEXT_EDIT_H

#include <QTextBlock>
#include <QTextEdit>
#include <QVector>

class QMainWindow;
class QTextDocument;

namespace console {

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    TextEdit(QWidget *parent = 0);
protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
private:
    // Data
    QTextDocument *_textDocument;
    QString _prompt;
    // Methods
    QMainWindow* mainWindow();
    void writePrompt();
    void insertText(QString text);
    int numLines();
    int linePosition(int i);
    int cursorPosition();
    int cursorLine();
    bool bounceCursor(bool sticky);
};

}

#endif
