#ifndef EGG_DEVELOPMENT_H
#define EGG_DEVELOPMENT_H
#include <base/box.h>

namespace student {

class EggDevelopment : public base::Box
{
public:
    EggDevelopment(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    QDate currentDate, fromDate, toDate;
    // Outputs
    bool on;
};

}

#endif
