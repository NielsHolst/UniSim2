#ifndef EXUDATION_H
#define EXUDATION_H
#include <base/box.h>

namespace saccharina {

class Exudation : public base::Box
{
public:
    Exudation(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double gamma, C, Cmin;
    // Outputs
    double value;
};

}

#endif
