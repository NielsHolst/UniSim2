#ifndef NITROGEN_CONTENT_H
#define NITROGEN_CONTENT_H
#include <base/box.h>

namespace saccharina {

class Nitrogencontent : public base::Box
{
public:
    Nitrogencontent(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Ninit, nitrogenreservesrate, dt;
    // Outputs
    double N;
};

}

#endif
