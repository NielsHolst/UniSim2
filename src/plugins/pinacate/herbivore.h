#ifndef Herbivore_H
#define Herbivore_H
#include <base/box.h>

namespace pinacate {

class Herbivore : public base::Box
{
public:
    Herbivore(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double weight;
    double popsize;
    // Outputs
    double value;
};

}

#endif
