#include <QFont>
#include <QFontDatabase>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextEdit>
#include <base/command_help.h>
#include <base/dialog_base.h>
#include <base/publish.h>
#include "clear.h"

using namespace base;

namespace command {

PUBLISH(clear)
HELP(clear, "clear", "clears console window")

clear::clear(QString name, QObject *parent)
    : Command(name, parent)
{
}

void clear::doExecute() {
    QTextCursor cursor = dialog().textEdit()->textCursor();
    QTextCharFormat format = cursor.charFormat();
    QFont font = format.font();
    QString family = font.family();
    int pt = font.pointSize();

    dialog().textEdit()->clear();

    format.setFont(QFontDatabase().font(family, QString(), pt));
    cursor.setCharFormat(format);
    dialog().textEdit()->setTextCursor(cursor);
}

}
