#include <base/dialog.h>
#include <base/publish.h>
#include "set.h"

using namespace base;

namespace command {

QFontDatabase set::_fontDatabase;

PUBLISH(set)

set::set(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(set);
}

void set::execute() {
    QString hint{"\nIf font name contains spaces, enclose it in apostrophes"};
    _cursor = dialog().textCursor();
    _format = _cursor.charFormat();
    _font = _format.font();
    int n = _args.size();
    if (n == 1) {
        dialog().error("Write: set <something>");
        return;
    }
    else if (n == 2) {
        QString a1 = _args.at(1);
        if (a1 == "font")
            showFont();
        else
            dialog().error("Unknown command: set " + a1);
    }
    else if (n == 3) {
        QString a1 = _args.at(1), a2 = _args.at(2);
        if (_args.at(1) == "font") {
            bool ok;
            double pt = a2.toDouble(&ok);
            if (ok)
                setFont(pt);
            else
                setFont(a2);
        }
        else
            dialog().error("Unknown command: set " + a1);
    }
    else if (n == 4) {
        QString a1 = _args.at(1), a2 = _args.at(2) , a3 = _args.at(3);
        if (_args.at(1) == "font") {
            bool ok;
            int pt = a3.toInt(&ok);
            if (ok)
                setFont(a2, pt);
            else
                dialog().error("Last argument must be a number: " + _args.join(" ") + hint);
        }
        else
            dialog().error("Unknown command: set " + a1);
    }
    else {
        QString s{"Too many arguments (%1): %2" + hint};
        dialog().error(s.arg(n).arg(_args.join(" ")));
    }

}

void set::showFont() {
    QString s{"%1 %2pt"};
    dialog().information(s.arg(_format.font().family()).arg(_format.fontPointSize()));
}

void set::setFont(int pt) {
    _font = _fontDatabase.font(_font.family(), QString(), pt);
    _format.setFont(_font);
    _cursor.setCharFormat(_format);
    dialog().setTextCursor(_cursor);
    showFont();
}

void set::setFont(QString family) {
    _font = _fontDatabase.font(family, QString(), _font.pointSize());
    _format.setFont(_font);
    _cursor.setCharFormat(_format);
    dialog().setTextCursor(_cursor);
    showFont();
}

void set::setFont(QString family, int pt) {
    _font = _fontDatabase.font(family, QString(), pt);
    _format.setFont(_font);
    _cursor.setCharFormat(_format);
    dialog().setTextCursor(_cursor);
    showFont();
}



}
