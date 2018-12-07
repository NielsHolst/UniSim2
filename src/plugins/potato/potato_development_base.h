#ifndef POTATO_DEVELOPMENT_BASE_H
#define POTATO_DEVELOPMENT_BASE_H
#include <base/box.h>

namespace potato {

class PotatoDevelopmentBase: public base::Box
{
public:
    PotatoDevelopmentBase(QString name, QObject *parent);
protected:
    // Inputs
    double timeStepDays;
    // Outputs
    double step,  total;
};

}
#endif
