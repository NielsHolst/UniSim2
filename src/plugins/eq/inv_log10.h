#ifndef INV_LOG10_H
#define INV_LOG10_H
#include <base/box.h>

namespace eq {

class InvLog10 : public base::Box
{
public:
    InvLog10(QString name, QObject *parent);
    void reset();
    void update();
protected:
    // input
    double x;
    // output
    double value;
};

} //namespace

#endif
