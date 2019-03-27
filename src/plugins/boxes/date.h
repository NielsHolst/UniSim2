#ifndef DATE_H
#define DATE_H
#include <QDate>
#include <base/box.h>

namespace boxes {

class Date : public base::Box
{
public:
    Date(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    int day, month, year;
    // Output
    QDate value;
};

}

#endif
