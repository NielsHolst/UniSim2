#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QKeyEvent>
#include <QMainWindow>
#include <QSettings>
#include <QStatusBar>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include "command.h"
#include "dialog.h"
#include "environment.h"
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
    setObjectName("dialog");
    setDocument(_textDocument = new QTextDocument(this));
    restoreFont();
    writeWelcome();
    writePrompt();
    QObject::connect(qApp, &QApplication::focusChanged,
                     this, &Dialog::receivedFocus);
}


Dialog::~Dialog() {
    saveFont();
}

void Dialog::saveFont() {
    QTextCursor cursor = textCursor();
    QTextCharFormat format = cursor.charFormat();
    QSettings settings;
    settings.setValue("font/family", format.font().family());
    settings.setValue("font/size", format.fontPointSize());
}

void Dialog::restoreFont() {
    QSettings settings;
    QString family = settings.value("font/family", QString()).toString();
    bool ok;
    int pt = settings.value("font/size", 0).toInt(&ok);
    if (!ok) pt = 0;

    QTextCursor cursor = textCursor();
    QTextCharFormat format = cursor.charFormat();
    format.setFont(QFontDatabase().font(family, QString(), pt));
    cursor.setCharFormat(format);
    setTextCursor(cursor);
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
        moveCursor(QTextCursor::End);
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
        cursor = getCursor();
        cursor.movePosition(QTextCursor::EndOfLine);
        setTextCursor(cursor);
        submitCommand();
        insertText("\n" + _prompt);
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
    mainWindow()->statusBar()->showMessage(QString::number(numLines()));
}

void Dialog::receivedFocus(QWidget *old, QWidget *now) {
    if (!old && now == this)
        moveCursor(QTextCursor::End);
}

QMainWindow* Dialog::mainWindow() {
    return dynamic_cast<QMainWindow*>(parent());
}

QTextCursor Dialog::getCursor() {
    QTextCursor cursor = dialog().textCursor();
    QTextCharFormat format = cursor.charFormat();
    QFont font = format.font();
    QString family = font.family();
    int pt = font.pointSize();
    format.setFont(QFontDatabase().font(family, QString(), pt));
    cursor.setCharFormat(format);
    return cursor;
}

void Dialog::writePrompt() {
   insertText("\n" + _prompt);
}

void Dialog::writeWelcome() {
    QString latestFile = environment().state.latestLoadArg,
            inputFolder = environment().state.dir.input.path(),
            workFolder = environment().state.dir.work.absolutePath(),
            info = latestFile.isEmpty() ? "Welcome to Universal Simulator." : "Welcome back";
    info += "\nYour work  folder is '" + workFolder + "'";
    info += "\nYour input folder is '" + inputFolder + "'";
    if (!latestFile.isEmpty())
        info += "\nYour latest file was '" + latestFile + "'";
    dialog().information(info);
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
    Command *command(0);
    QTextBlock block = _textDocument->findBlock(cursorPosition());
    QString line = block.text().mid(_prompt.size()).simplified();
    QStringList items;
    try {
        items = base::split(line);
        if (!items.isEmpty()) {
            try {
                if (items.first().contains("_"))
                    throw Exception("Illegal characted in command");
                command = MegaFactory::create<Command>(items.first(), items.first(), this);
            }
            catch (Exception &ex) {
                error("Unknown command: '" + line + "'");
                command = 0;
            }
        }
    }
    catch(Exception &ex) {
        error(ex.what());
        command = 0;
    }

    if (command) {
        command->arguments(items);
        command->execute();
    }
    if (!line.isEmpty())
        _history.add(line);
}

}
