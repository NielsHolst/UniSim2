#ifndef THRESHOLD_SWEDEN_H
#define THRESHOLD_SWEDEN_H
#include "threshold_base.h"

namespace aphid {

class ThresholdSweden : public ThresholdBase
{
public:
    ThresholdSweden(QString name, QObject *parent);
    void update();
};

}

#endif
