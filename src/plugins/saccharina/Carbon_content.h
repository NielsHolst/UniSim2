#ifndef CARBON_CONTENT_H
#define CARBON_CONTENT_H
#include <base/box.h>

namespace saccharina {

class Carboncontent : public base::Box
{
public:
    Carboncontent(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Cinit, carbonreservesrate, dt;
    // Outputs
    double C;
};

}

#endif
