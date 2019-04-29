#ifndef THRESHOLD_NORWAY_H
#define THRESHOLD_NORWAY_H
#include "threshold_base.h"

namespace aphid {

class ThresholdNorway : public ThresholdBase
{
public:
    ThresholdNorway(QString name, QObject *parent);
    void update();
};

}

#endif
