#ifndef POTATO_DEVELOPMENT_BY_HOUR_H
#define POTATO_DEVELOPMENT_BY_HOUR_H
#include "potato_development_base.h"

namespace potato {

class PotatoDevelopmentByHour : public PotatoDevelopmentBase
{
public:
    PotatoDevelopmentByHour(QString name, QObject *parent);
    void reset();
    void update();
private:
    //inputs
    double temp;
};

}

#endif
