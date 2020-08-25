/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QApplication>
#include <QFileDialog>
#include <QFont>
#include <QFontDatabase>
#include <QKeyEvent>
#include <QMainWindow>
#include <QProgressBar>
#include <QSettings>
#include <QStatusBar>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include "command.h"
#include "dialog_widget.h"
#include "environment.h"
#include "exception.h"
#include "general.h"
#include "mega_factory.h"

#include <QMessageBox>

namespace base {

DialogWidget::DialogWidget(QMainWindow *parent)
    : QTextEdit(parent),
      _init(false),
      _prompt("> "),
      _informationColor(QColor("blue")),
      _errorColor(QColor("red")),
      _firstInformation(true),
      _gotoEnd(false)
{
    setObjectName("dialog");
    setDocument(_textDocument = new QTextDocument(this));
    setFocus(Qt::ActiveWindowFocusReason);
}

void DialogWidget::init() {
    if (_init)
        ThrowException("Call DialogWidget::init() only once");
    restoreFont();
    writeWelcome();
    writePrompt();
    setAcceptRichText(false);
    _progressBar = new QProgressBar(this);
    _progressBar->hide();
    mainWindow()->statusBar()->addPermanentWidget(_progressBar);
    QObject::connect(qApp, &QApplication::focusChanged,
                     this, &DialogWidget::receivedFocus);
    _init = true;
}

DialogWidget::~DialogWidget() {
    saveFont();
}


void DialogWidget::saveFont() {
    QTextCursor cursor = textCursor();
    QTextCharFormat format = cursor.charFormat();
    QSettings settings;
    settings.setValue("font/family", format.font().family());
    settings.setValue("font/size", format.fontPointSize());
}

void DialogWidget::setFont(QString family, int pt) {
    // Construct font
    QString family2 = fontExists(family) ? family : preferredFamily();
    QFont font = QFontDatabase().font(family2, QString(), pt);

    // Set cursor to use font
    QTextCursor cursor = textCursor();
    QTextCharFormat format = cursor.charFormat();
    format.setFont(font);
    cursor.setCharFormat(format);
    setTextCursor(cursor);
}

QFont DialogWidget::font() {
    QTextCursor cursor = textCursor();
    QTextCharFormat format = cursor.charFormat();
    return format.font();
}

inline QString defaultFamily() {
    return QGuiApplication::font().family();
}

void DialogWidget::restoreFont() {
    QSettings settings;
    QString family = resetFont() ?
                     preferredFamily() :
                     settings.value("font/family", preferredFamily()).toString();
    int pt = settings.value("font/size", 11).toInt();

    QTextCursor cursor = textCursor();
    QTextCharFormat format = cursor.charFormat();
    format.setFont(QFontDatabase().font(family, QString(), pt));
    cursor.setCharFormat(format);
    setTextCursor(cursor);
}

bool DialogWidget::resetFont() {
    QString stored = QSettings().value("font/family").toString();
    return stored.isEmpty() || stored == defaultFamily();
}

QString DialogWidget::preferredFamily() {
    QList<QString> preferred;
    preferred
        << "InputMonoCompressed Light"
        << "Consolas"
        << "Andale Mono"
        << "Menlo"
        << "Courier";
    for (QString family : preferred) {
        if (fontExists(family))
            return family;
    }
    return QGuiApplication::font().family();
}

bool DialogWidget::fontExists(QString family) {
    return QFontDatabase().font(family, QString(), 10).family() !=
           QGuiApplication::font().family();
}

QProgressBar* DialogWidget::progressBar() {
    Q_ASSERT(_progressBar);
    return _progressBar;
}

void DialogWidget::finished() {
    _progressBar->hide();
    qApp->processEvents();
}

void DialogWidget::message(QString s) {
    mainWindow()->statusBar()->showMessage(s);
    mainWindow()->repaint();
    repaint();
}

void DialogWidget::information(QString s) {
    if (_firstInformation)
        _firstInformation = false;
    else
        s = "\n" + s;

    insertText(s, _informationColor);
    repaint();
}

void DialogWidget::errorImpl(QString s) {
    insertText("\n" + s, _errorColor);
    repaint();
}

void DialogWidget::keyPressEvent(QKeyEvent *event) {
    auto controlModifier = environment().isMac() ? Qt::MetaModifier : Qt::ControlModifier;
    if (event->modifiers() & controlModifier)
        handleCtrlKey(event);
    else
        handleNormalKey(event);
}

void DialogWidget::handleCtrlKey(QKeyEvent *event) {
    QTextCursor cursor;
    QString s;
    switch (event->key()) {
    case Qt::Key_L:
//        event->accept();
//        restoreFont();
        Command::submit(QStringList() << "clear", this);
//        handleEscKey();

//        event->accept();
//        moveCursor(QTextCursor::End);
//        cursor = getCursor();
//        setTextCursor(cursor);
//        Command::submit(QStringList() << "clear", this);
//        insertText("\n" + _prompt);
//        submitCommand();
        break;
    case Qt::Key_Space:
        s = selectFile();
        if (!s.isEmpty()) {
            QStringList items = lineItems();
            if (items.size() > 1) {
                clearLine();
                items.removeAt(items.size() - 1);
                items << s;
                s = items.join(" ");
            }
            else {
                s.prepend(" ");
            }
            insertText(s);
            submitCommand();
        }
        break;
    default:
        QTextEdit::keyPressEvent(event);
        break;
    }
}

void DialogWidget::loadWithFilePicker() {
    QString s = selectFile();
    if (!s.isEmpty()) {
        s.prepend(" ");
        insertText(s);
        submitCommand();
    }
}

QString DialogWidget::selectFile() {
    QStringList items = lineItems();
    QString writtenPath = (items.size() > 1) ? items.last() : "",
            inputPath = environment().resolveDir(Environment::Input).absolutePath(),
//            latestFile = environment().latestLoadArg(),
//            latestPath = QFileInfo(latestFile).path(),
            latestPath = environment().latestInputFilePath(),
            usePath = writtenPath.isEmpty() ? latestPath : writtenPath,
            combinedPath = QDir::isAbsolutePath(usePath) ? usePath : inputPath + "/" + usePath,
            path = QDir(combinedPath).exists() ? combinedPath : inputPath;
    QString filePath = QFileDialog::getOpenFileName(this,
            "Select a file", path, "Scripts (*.box *.xml)");
    if (filePath.startsWith(inputPath))
        filePath = filePath.mid(inputPath.size() + 1);
    if (filePath.contains(" "))
        filePath = "\"" + filePath + "\"";
    return filePath;
}

void DialogWidget::handleNormalKey(QKeyEvent *event) {
    _gotoEnd = false;
    QTextCursor cursor;
    switch (event->key()) {
    case Qt::Key_Escape:
        event->accept();
        handleEscKey();
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

void DialogWidget::handleEscKey() {
    moveCursor(QTextCursor::End);
    writePrompt();
}

void DialogWidget::receivedFocus(QWidget *old, QWidget *now) {
    _gotoEnd = (!old && now == this);
}

void DialogWidget::mousePressEvent(QMouseEvent *event) {
    if (_gotoEnd)  {
        event->accept();
        moveCursor(QTextCursor::End);
        _gotoEnd = false;
    }
    else {
        QTextEdit::mousePressEvent(event);
    }
}


QMainWindow* DialogWidget::mainWindow() {
    return dynamic_cast<QMainWindow*>(parent());
}

QTextCursor DialogWidget::getCursor() {
    QTextCursor cursor = textCursor();
    QTextCharFormat format = cursor.charFormat();
    QFont font = format.font();
    QString family = font.family();
    int pt = font.pointSize();
    format.setFont(QFontDatabase().font(family, QString(), pt));
    cursor.setCharFormat(format);
    return cursor;
}

void DialogWidget::writePrompt() {
   insertText("\n" + _prompt);
}

void DialogWidget::writeWelcome() {
    information(environment().isFirstInstallation() ?
                "\nWelcome to Universal Simulator!" : "\nWelcome back!");

    environment().computationStep(ComputationStep::Start);

    information("Saving box script grammar...");
    Command::submit(QStringList() << "save" << "grammar", this);

    QString info = "\nWork folder:\n  " + environment().folderInfo(Environment::Work) +
                   "\nInput folder:\n  " + environment().folderInfo(Environment::Input);

    QString latestFile = environment().latestLoadArg();
    if (latestFile.contains(" "))
        latestFile = "\"" + latestFile + "\"";
    if (latestFile.length() > 2)
        info += "\nYour latest file was " + latestFile;
    _history.add("load " + latestFile);
    information(info);

    environment().computationStep(ComputationStep::Ready);
}


void DialogWidget::insertText(QString text, QColor color) {
    setTextColor(color);
    QTextCursor cursor = textCursor();
    cursor.insertText(text);
    setTextCursor(cursor);
}

int DialogWidget::numLines() {
    return _textDocument->lineCount();
}

int DialogWidget::linePosition(int i) {
    Q_ASSERT(i<numLines());
    return _textDocument->findBlockByLineNumber(i).position();
}

int DialogWidget::cursorPosition() {
    return textCursor().position();
}

int DialogWidget::cursorLine() {
    for (int i = numLines()-1; i>=0; --i) {
        if (cursorPosition() > linePosition(i))
            return i;
    }
    // Rarely, we end here
    return 0;
}

bool DialogWidget::bounceCursor(bool sticky) {
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

void DialogWidget::clearLine() {
    QTextCursor cursor;
    while (!bounceCursor(true)) {
        cursor = textCursor();
        cursor.movePosition(QTextCursor::EndOfLine);
        cursor.deletePreviousChar();
        setTextCursor(cursor);
    }
}

void DialogWidget::submitCommand() {
    QTextCursor cursor = getCursor();
    cursor.movePosition(QTextCursor::EndOfLine);
    setTextCursor(cursor);

    QString line = DialogWidget::line();
    QStringList items = lineItems();
    if (!items.isEmpty())
        Command::submit(items, this);
    if (!line.isEmpty())
        _history.add(line);

    writePrompt();
}

QString DialogWidget::line() {
    QTextBlock block = _textDocument->findBlock(cursorPosition());
    return block.text().mid(_prompt.size()).simplified();
}

QStringList DialogWidget::lineItems() {
    return base::split(line());
}

}
