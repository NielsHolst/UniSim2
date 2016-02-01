#include <base/dialog.h>
#include <base/publish.h>
#include "set_font.h"

using namespace base;

namespace command {

QFontDatabase set_font::_fontDatabase;

PUBLISH(set_font)

set_font::set_font(QString name, QObject *parent)
    : set(name, parent)
{
    Class(set_font);
}

void set_font::execute() {
    _cursor = dialog().textCursor();
    _format = _cursor.charFormat();
    _font = _format.font();
    bool ok;
    int pt;
    QStringList &a(_args);
    QString hint{"\nIf font name contains spaces, enclose it in apostrophes"};

    switch(a.size()) {
    case 2:
        showFont();
        break;
    case 3:
        pt = a[2].toInt(&ok);
        if (ok)
            setFont(pt);
        else
            setFont(a[2]);
        break;
    case 4:
        pt = a[3].toInt(&ok);
        if (ok)
            setFont(a[2], pt);
        else
            dialog().error("Last argument must be a number: " + _args.join(" ") + hint);
        break;
    default:
        QString s{"Too many arguments (%1): %2" + hint};
        dialog().error(s.arg(a.size()).arg(a.join(" ")));
    }
}

void set_font::showFont() {
    QString s{"%1 %2pt"};
    dialog().information(s.arg(_format.font().family()).arg(_format.fontPointSize()));
}

void set_font::setFont(int pt) {
    _font = _fontDatabase.font(_font.family(), QString(), pt);
    _format.setFont(_font);
    _cursor.setCharFormat(_format);
    dialog().setTextCursor(_cursor);
    showFont();
}

void set_font::setFont(QString family) {
    _font = _fontDatabase.font(family, QString(), _font.pointSize());
    _format.setFont(_font);
    _cursor.setCharFormat(_format);
    dialog().setTextCursor(_cursor);
    showFont();
}

void set_font::setFont(QString family, int pt) {
    _font = _fontDatabase.font(family, QString(), pt);
    _format.setFont(_font);
    _cursor.setCharFormat(_format);
    dialog().setTextCursor(_cursor);
    showFont();
}



}
