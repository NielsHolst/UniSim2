#ifndef F_NITROGEN_H
#define F_NITROGEN_H
#include <base/box.h>

namespace saccharina {

class Fnitrogen : public base::Box
{
public:
    Fnitrogen(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double N, Nmin, Nmax;
    // Outputs
    double value;
};

}

#endif
