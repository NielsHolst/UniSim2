#ifndef POTATO_DEVELOPMENT_BY_DAY_H
#define POTATO_DEVELOPMENT_BY_DAY_H
#include "potato_development_base.h"

namespace potato {

class PotatoDevelopmentByDay: public PotatoDevelopmentBase
{
public:
    PotatoDevelopmentByDay(QString name, QObject *parent);
    void reset();
    void update();
private:
    //inputs
    double minStep, maxStep;

};

}

#endif
