#include <QKeyEvent>
#include <QMainWindow>
#include <QStatusBar>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include "text_edit.h"

namespace console {

TextEdit::TextEdit(QWidget *parent)
    : QTextEdit(parent), _prompt("> ")
{
    setDocument(_textDocument = new QTextDocument(this));
    writePrompt();
}

void TextEdit::keyPressEvent(QKeyEvent *event) {
    QStringList s;
    for (int i=0; i<numLines(); ++i)
        s << QString::number(linePosition(i));

    QTextCursor cursor = textCursor();
    int cpos = cursor.position();


    QString msg("n: %1 cpos: %2 cline: %3 L: %4 [%5]");
    mainWindow()->statusBar()->showMessage(msg.
                                           arg(_textDocument->characterCount()).
                                           arg(QString::number(cpos)).
                                           arg(QString::number(cursorLine())).
                                           arg(numLines()).
                                           arg(s.join(" ")));
    switch (event->key()) {
    case Qt::Key_Escape:
        event->accept();
        cursor = textCursor();
        cursor.movePosition(QTextCursor::EndOfLine);
        setTextCursor(cursor);
        writePrompt();
        break;
    case Qt::Key_Tab:
        event->accept();
        break;
    case Qt::Key_Backspace:
        if (bounceCursor(true))
            event->accept();
        else
            QTextEdit::keyPressEvent(event);
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        event->accept();
        insertText("EXEC");
        cursor = textCursor();
        cursor.movePosition(QTextCursor::EndOfLine);
        setTextCursor(cursor);
        writePrompt();
        break;
    case Qt::Key_Insert:
    case Qt::Key_Pause:
        event->accept();
        break;
    case Qt::Key_Home:
        event->accept();
        cursor = textCursor();
        cursor.setPosition(linePosition(cursorLine()) + _prompt.size());
        setTextCursor(cursor);
        break;
    case Qt::Key_Left:
        if (bounceCursor(true))
            event->accept();
        else
            QTextEdit::keyPressEvent(event);
        break;
    case Qt::Key_Right:
        if (bounceCursor(false))
            event->accept();
        else
            QTextEdit::keyPressEvent(event);
        break;
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
        event->accept();
        break;
    case Qt::Key_Up:
        event->accept();
        insertText("UP");
        break;
    case Qt::Key_Down:
        event->accept();
        insertText("DN");
        break;
    default:
        QTextEdit::keyPressEvent(event);
        break;
    }
}

QMainWindow* TextEdit::mainWindow() {
    return dynamic_cast<QMainWindow*>(parent());
}

void TextEdit::writePrompt() {
   insertText("\n> ");
}

void TextEdit::insertText(QString text) {
    QTextCursor cursor = textCursor();
    cursor.insertText(text);
    setTextCursor(cursor);
}

int TextEdit::numLines() {
    return _textDocument->lineCount();
}

int TextEdit::linePosition(int i) {
    Q_ASSERT(i<numLines());
    return _textDocument->findBlockByLineNumber(i).position();
}

int TextEdit::cursorPosition() {
    return textCursor().position();
}

int TextEdit::cursorLine() {
    for (int i = numLines()-1; i>=0; --i) {
        if (cursorPosition() > linePosition(i))
            return i;
    }
    Q_ASSERT(false);
}

bool TextEdit::bounceCursor(bool sticky) {
    int start = linePosition(cursorLine()) + _prompt.size();
    QTextCursor cursor = textCursor();
    int pos = cursor.position();
    int bounce = (sticky && pos <= start) || (!sticky && pos < start);
    if (bounce) {
        cursor.setPosition(start);
        setTextCursor(cursor);
        return true;
    }
    return false;
}


}
