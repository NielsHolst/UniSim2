#ifndef SET_FONT_H
#define SET_FONT_H
#include <QFont>
#include <QFontDatabase>
#include <QTextCharFormat>
#include <QTextCursor>
#include "set.h"

namespace command {

class set_font : public set
{
public:
    set_font(QString name, QObject *parent);
    void execute();
private:
    static QFontDatabase _fontDatabase;
    QFont _font;
    QTextCursor _cursor;
    QTextCharFormat _format;
    void showFont();
    void setFont(int pt);
    void setFont(QString family);
    void setFont(QString family, int pt);
};

}

#endif
