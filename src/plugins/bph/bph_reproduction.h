#ifndef BPH_REPRODUCTION_H
#define BPH_REPRODUCTION_H
#include <base/box.h>

namespace bph {

class BphReproduction : public base::Box
{
public:
    BphReproduction(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double max, vegetative, reproductive, ripening, repScale, ripScale;
    // Data
    double value;
};

}

#endif
