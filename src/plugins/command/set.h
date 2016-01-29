#ifndef SET_H
#define SET_H
#include <QFont>
#include <QFontDatabase>
#include <QTextCharFormat>
#include <QTextCursor>
#include <base/command.h>

namespace command {

class set : public base::Command
{
public:
    set(QString name, QObject *parent);
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
