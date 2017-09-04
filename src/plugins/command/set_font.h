#ifndef SET_FONT_H
#define SET_FONT_H
#include "set.h"

namespace command {

class set_font : public set
{
public:
    set_font(QString name, QObject *parent);
private:
    // Data
    QString _previousFamily;
    int _previousPointSize;
    // Methods
    void doExecute();
    QString currentFamily();
    int currentPointSize();
    void showFont();
    void showFontUnchanged();
    void setFont(int pt);
    void setFont(QString family);
    void setFont(QString family, int pt);
};

}

#endif
