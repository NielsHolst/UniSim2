#ifndef REPLICATOR_H
#define REPLICATOR_H
#include <base/box.h>

namespace boxes {

class Replicator : public base::Box
{
public:
    Replicator(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    int step, steps, replicates;
    // Output
    int replicate;
};

}

#endif
