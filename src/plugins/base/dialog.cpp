#include <QKeyEvent>
#include <QMainWindow>
#include <QStatusBar>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include "command.h"
#include "dialog.h"
#include "exception.h"
#include "general.h"
#include "mega_factory.h"

namespace base {

Dialog::Dialog(QWidget *parent)
    : DialogBase(parent),
      _prompt("> "),
      _informationColor(QColor("blue")),
      _errorColor(QColor("red"))
{
    setDocument(_textDocument = new QTextDocument(this));
    writePrompt();
}

void Dialog::information(QString s) {
    insertText("\n" + s, _informationColor);
}

void Dialog::error(QString s) {
    insertText("\n" + s, _errorColor);
}

void Dialog::keyPressEvent(QKeyEvent *event) {
    QTextCursor cursor;
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
        submitCommand();
        cursor = textCursor();
        cursor.movePosition(QTextCursor::EndOfLine);
        setTextCursor(cursor);
        writePrompt();
        mainWindow()->statusBar()->showMessage(QString::number(numLines()));
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
        clearLine();
        insertText(_history.previous());
        break;
    case Qt::Key_Down:
        event->accept();
        clearLine();
        insertText(_history.next());
        break;
    default:
        QTextEdit::keyPressEvent(event);
        break;
    }
}

QMainWindow* Dialog::mainWindow() {
    return dynamic_cast<QMainWindow*>(parent());
}

void Dialog::writePrompt() {
   insertText("\n> ");
}

void Dialog::insertText(QString text, QColor color) {
    setTextColor(color);
    QTextCursor cursor = textCursor();
    cursor.insertText(text);
    setTextCursor(cursor);
}

int Dialog::numLines() {
    return _textDocument->lineCount();
}

int Dialog::linePosition(int i) {
    Q_ASSERT(i<numLines());
    return _textDocument->findBlockByLineNumber(i).position();
}

int Dialog::cursorPosition() {
    return textCursor().position();
}

int Dialog::cursorLine() {
    for (int i = numLines()-1; i>=0; --i) {
        if (cursorPosition() > linePosition(i))
            return i;
    }
    Q_ASSERT(false);
}

bool Dialog::bounceCursor(bool sticky) {
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

void Dialog::clearLine() {
    QTextCursor cursor;
    while (!bounceCursor(true)) {
        cursor = textCursor();
        cursor.movePosition(QTextCursor::EndOfLine);
        cursor.deletePreviousChar();
        setTextCursor(cursor);
    }
}

void Dialog::submitCommand() {
    Command *command;
    QTextBlock block = _textDocument->findBlock(cursorPosition());
    QString line = block.text().mid(_prompt.size()).simplified();
    QStringList items;
    try {
        items = base::split(line);
    }
    catch(Exception &ex) {
        error(ex.what());
        return;
    }

    if (!items.isEmpty()) {
        try {
            command = MegaFactory::create<Command>(items.first(), items.first(), this);
        }
        catch (Exception &ex) {
            error(QString("Unknown command: '%1'").arg(items.first()));
            return;
        }
    }
    Q_ASSERT(command);
    command->arguments(items);
    command->execute(this, &_environment);
    _history.add(line);
}

}
