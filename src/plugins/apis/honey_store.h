#ifndef HONEY_STORE_H
#define HONEY_STORE_H
#include <base/box.h>

namespace apis {

class HoneyStore : public base::Box
{
public:
    HoneyStore(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    double change, capacity, initial;
    // output
    double holding;
};

}

#endif
