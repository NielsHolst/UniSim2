#ifndef PARASITOID_H
#define PARASITOID_H
#include <base/box.h>
#include <QDate>

namespace tuta {

class Parasitoid : public base::Box
{
public:
    Parasitoid(QString name, QObject *parent);
    void update();
private:
    // Inputs
    QDate  firstDate, date;
    double number;
    // Outputs
    double value;
};

}

#endif
