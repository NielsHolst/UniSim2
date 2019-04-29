#ifndef APHID_INDEX_H
#define APHID_INDEX_H
#include <base/box.h>

namespace aphid {

class AphidIndex : public base::Box
{
public:
    AphidIndex(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double nymphs, adults;
    // Outputs
    double value;
};

}

#endif
