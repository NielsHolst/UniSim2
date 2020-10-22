#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <base/box.h>

namespace saccharina {

class Structure : public base::Box
{
public:
    Structure(QString name, QObject *parent);
private:
    // Inputs
    double proportionC, proportionN;
    // Outputs
    double mass, senescent, respired;
};

}

#endif
