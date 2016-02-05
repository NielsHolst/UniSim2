#include <QFont>
#include <QFontDatabase>
#include <QTextCharFormat>
#include <QTextCursor>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/publish.h>
#include "clear.h"

using namespace base;

namespace command {

PUBLISH(clear)
HELP(clear, "clear", "clears console window")

clear::clear(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(clear);
}

void clear::doExecute() {
    QTextCursor cursor = dialog().textCursor();
    QTextCharFormat format = cursor.charFormat();
    QFont font = format.font();
    QString family = font.family();
    int pt = font.pointSize();

    dialog().clear();

    format.setFont(QFontDatabase().font(family, QString(), pt));
    cursor.setCharFormat(format);
    dialog().setTextCursor(cursor);
}

}
